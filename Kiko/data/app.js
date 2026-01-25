// Kiko AI Assistant - WebSocket Communication & UI Controller
// Communicates with ESP32 via WebSocket for real-time updates

// ========== GLOBAL STATE ==========
const state = {
  currentTab: 'status',
  ws: null,
  wsAttempts: 0,
  maxWsAttempts: 10,
  wsRetryDelay: 3000,
  lastState: null,
  alarmInterval: null,
};

// ========== INITIALIZATION ==========
document.addEventListener('DOMContentLoaded', () => {
  initTabNavigation();
  initWebSocket();
  updateUI(); // Initial render
});

// ========== WEBSOCKET CONNECTION ==========
function initWebSocket() {
  const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
  // WebSocket server on ESP32 runs on port 81 (separate from HTTP server on port 80)
  const host = window.location.hostname;
  const wsUrl = `${protocol}//${host}:81`;
  
  try {
    state.ws = new WebSocket(wsUrl);
    
    state.ws.onopen = () => {
      console.log('✅ WebSocket connected');
      state.wsAttempts = 0; // Reset retry counter
      updateStatusIndicator(true);
    };
    
    state.ws.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        console.log('📨 WebSocket message received:', data); // DEBUG
        handleMessage(data);
      } catch (e) {
        console.error('Failed to parse WebSocket message:', e, 'Raw:', event.data);
      }
    };
    
    state.ws.onerror = (error) => {
      console.error('WebSocket error:', error);
      updateStatusIndicator(false);
    };
    
    state.ws.onclose = () => {
      console.log('WebSocket disconnected, retrying...');
      updateStatusIndicator(false);
      reconnectWebSocket();
    };
    
  } catch (e) {
    console.error('Failed to create WebSocket:', e);
    reconnectWebSocket();
  }
}

function reconnectWebSocket() {
  if (state.wsAttempts < state.maxWsAttempts) {
    state.wsAttempts++;
    setTimeout(() => {
      initWebSocket();
    }, state.wsRetryDelay);
  }
}

// ========== MESSAGE HANDLING ==========
function handleMessage(data) {
  // Load chat history (on new connection) - show only last 5 chats
  if (data.history && Array.isArray(data.history)) {
    console.log('📚 Loading chat history:', data.history.length, 'messages');
    const chatList = document.getElementById('chatList');
    if (chatList) {
      chatList.innerHTML = ''; // Clear existing
      
      // Filter to only user and assistant messages (skip tool messages)
      const filteredMessages = data.history.filter(msg => msg.role === 'user' || msg.role === 'assistant');
      
      if (filteredMessages.length === 0) {
        chatList.innerHTML = '<p class="empty">No messages yet. Start talking to Kiko!</p>';
      } else {
        // Show only last 10 messages (approximately 5 user-assistant pairs)
        const lastMessages = filteredMessages.slice(-10);
        lastMessages.forEach(msg => {
          console.log('  ➕ Adding message:', msg.role, '-', msg.content.substring(0, 50));
          addChatMessage(msg.role, msg.content);
        });
      }
    } else {
      console.error('❌ chatList element not found!');
    }
  }
  
  // Update state
  if (data.state) {
    updateState(data.state);
  }
  
  // Update transcription
  if (data.transcript !== undefined) {
    updateTranscription(data.transcript);
  }
  
  // Update alarm
  if (data.alarm_time !== undefined || data.alarm !== undefined || data.is_ringing !== undefined) {
    updateAlarm(data.alarm_time, data.alarm, data.is_ringing, data.remaining);
  }
  
  // Update to-do lists
  if (data.lists) {
    updateTodoLists(data.lists);
  }
  
  // Update camera visibility
  if (data.camera_mode !== undefined) {
    updateCameraMode(data.camera_mode);
  }
  
  // Update chat message (real-time single message)
  if (data.message) {
    addChatMessage(data.message.role, data.message.content);
  }
  
  // Image captured notification
  if (data.image_ready) {
    loadLastCapturedImage();
  }
  
  // Update gallery with captured images
  if (data.gallery && Array.isArray(data.gallery)) {
    console.log('🖼️ Loading gallery:', data.gallery.length, 'images');
    const gallery = document.getElementById('gallery');
    if (gallery) {
      gallery.innerHTML = ''; // Clear existing
      if (data.gallery.length === 0) {
        gallery.innerHTML = '<div class="empty"><p>No images yet. Ask "What do you see?" to capture an image.</p></div>';
      } else {
        data.gallery.forEach(img => {
          loadLastCapturedImage();
        });
      }
    }
  }
  
  // Update gallery if image captured (legacy support)
  if (data.image_url) {
    addGalleryImage(data.image_url);
  }
}


// ========== STATE UPDATES ==========
function updateState(newState) {
  if (state.lastState === newState) return; // Skip if no change
  
  state.lastState = newState;
  const badge = document.getElementById('stateBadge');
  if (!badge) return;
  
  // Update text
  badge.textContent = newState;
  
  // Remove all state classes
  badge.className = 'state-badge';
  
  // Add appropriate class
  switch (newState.toLowerCase()) {
    case 'idle':
      badge.classList.add('state-idle');
      break;
    case 'listening':
      badge.classList.add('state-listening');
      break;
    case 'thinking':
      badge.classList.add('state-thinking');
      break;
    case 'speaking':
      badge.classList.add('state-speaking');
      break;
    case 'alarming':
      badge.classList.add('state-alarming');
      break;
    case 'surveillance':
      badge.classList.add('state-surveillance');
      break;
    default:
      badge.classList.add('state-idle');
  }
}

function updateTranscription(text) {
  const element = document.getElementById('transcription');
  if (!element) return;
  
  if (text.trim()) {
    element.textContent = text;
    element.classList.add('active');
  } else {
    element.textContent = 'Waiting for voice input...';
    element.classList.remove('active');
  }
}

function updateAlarm(alarmTime, isActive, isRinging, serverRemaining) {
  const element = document.getElementById('alarmCountdown');
  const clearBtn = document.getElementById('clearAlarmBtn');
  if (!element) return;
  
  // Clear existing interval
  if (state.alarmInterval) {
    clearInterval(state.alarmInterval);
    state.alarmInterval = null;
  }
  
  if (!isActive || !alarmTime) {
    element.textContent = 'No alarm set';
    element.className = 'alarm-inactive';
    if (clearBtn) clearBtn.style.display = 'none';
    return;
  }
  
  // Show clear button when alarm is active
  if (clearBtn) clearBtn.style.display = 'block';
  
  // If alarm is ringing, show "ALARM"
  if (isRinging) {
    element.textContent = 'ALARM';
    element.classList.remove('alarm-inactive');
    element.classList.add('alarm-ringing');
    return;
  }
  
  // Otherwise show countdown timer
  element.classList.remove('alarm-inactive', 'alarm-ringing');
  element.classList.add('active');
  
  // Use server-provided remaining time as base to avoid clock sync issues
  let baseRemaining = serverRemaining ? serverRemaining * 1000 : 0;
  let lastUpdateTime = Date.now();
  
  function updateCountdown() {
    const now = Date.now();
    const elapsed = now - lastUpdateTime;
    baseRemaining = Math.max(0, baseRemaining - elapsed);
    lastUpdateTime = now;
    
    if (baseRemaining <= 0) {
      element.textContent = '00:00';
      element.classList.add('alarm-about-to-ring');
      clearInterval(state.alarmInterval);
      return;
    }
    
    const minutes = Math.floor(baseRemaining / 60000);
    const seconds = Math.floor((baseRemaining % 60000) / 1000);
    element.textContent = `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`;
  }
  
  updateCountdown(); // Immediate update
  state.alarmInterval = setInterval(updateCountdown, 1000); // Update every second
}

// Updated Clear Alarm Function using Custom Modal
function clearAlarm() {
    showConfirm("Do you want to cancel the currently active alarm?", async () => {
        try {
            const response = await fetch('/api/alarm/cancel', { method: 'POST' });
            if (response.ok) {
                // UI will be updated automatically via WebSocket, 
                // but we can hide the button immediately for a better feel
                const clearBtn = document.getElementById('clearAlarmBtn');
                if (clearBtn) clearBtn.style.display = 'none';
                console.log("Alarm cancellation request sent");
            }
        } catch (e) {
            console.error("Failed to cancel alarm:", e);
        }
    }, "Cancel Alarm");
}

function updateTodoLists(lists) {
  const container = document.getElementById('todoLists');
  if (!container) return;
  
  console.log('📋 Updating todo lists:', lists);  // DEBUG
  
  // Empty check
  if (!lists || Object.keys(lists).length === 0) {
    container.innerHTML = '<div class="empty"><p>No to-do lists created yet.</p></div>';
    return;
  }
  
  let html = '';
  
  for (const [listName, items] of Object.entries(lists)) {
    html += `<div class="todo-list">
      <h3>${escapeHtml(listName)}</h3>`;
    
    if (items && Object.keys(items).length > 0) {
      html += '<div>';
      for (const [itemName, quantity] of Object.entries(items)) {
        const qty = parseInt(quantity) || 0;  // Ensure it's a number
        console.log(`  Item: "${itemName}" | Qty: ${qty}`);  // DEBUG
        html += `<div class="todo-item">
          <span>${escapeHtml(itemName)}</span>`;
        html += `<span class="qty">(${qty})</span>`;
        html += '</div>';
      }
      html += '</div>';
    } else {
      html += '<p style="color: #999; font-size: 0.9em;">List is empty</p>';
    }
    
    html += '</div>';
  }
  
  container.innerHTML = html;
}

function updateCameraMode(mode) {
  const container = document.getElementById('cameraContainer');
  if (!container) return;
  
  // Clear any existing refresh interval
  if (window.cameraRefreshInterval) {
    clearInterval(window.cameraRefreshInterval);
    window.cameraRefreshInterval = null;
  }
  
  if (mode === 'live' || mode === 'surveillance') {
    // Show continuous MJPEG live stream
    container.innerHTML = '<img id="cameraFeed" src="/stream" alt="Live Camera Feed" style="width: 100%; max-width: 600px; border-radius: 8px; margin-top: 10px; background: #000;">';
    container.classList.add('active');
    
    // Auto-switch to camera tab when surveillance mode starts
    switchTab('camera');
    
    console.log('📹 Surveillance mode: Continuous MJPEG stream activated');
  } else {
    // Hide camera feed
    container.classList.remove('active');
    container.innerHTML = '<div class="empty"><p>Camera not active. Enable surveillance mode to view.</p></div>';
    console.log('🎥 Surveillance mode stopped: Stream deactivated');
  }
}


// ========== CHAT FUNCTIONS ==========
function addChatMessage(role, content) {
  // Only add user and assistant messages, skip tool messages
  if (role !== 'user' && role !== 'assistant') {
    console.log('⏭️  Skipping non-chat message (role=' + role + ')');
    return;
  }
  
  const list = document.getElementById('chatList');
  if (!list) return;
  
  // Skip displaying JSON tool call responses
  if (role === 'assistant') {
    try {
      const parsed = JSON.parse(content);
      // If it's a tool call JSON (has tool_calls), skip displaying it
      if (parsed.tool_calls) {
        console.log('⏭️  Skipping tool call response JSON');
        return;
      }
    } catch (e) {
      // Not JSON, continue with normal display
    }
  }
  
  // Remove "no messages" placeholder if exists
  const empty = list.querySelector('.empty');
  if (empty) empty.remove();
  
  const msg = document.createElement('div');
  msg.className = `chat-msg chat-${role === 'user' ? 'user' : 'ai'}`;
  
  // Set icon based on role
  const icon = role === 'user' ? '👤' : '🤖';
  
  // Create message with icon
  msg.innerHTML = `<span class="msg-icon">${icon}</span><span class="msg-text">${escapeHtml(content)}</span>`;
  
  // Keep only last 10 messages (approximately 5 user-assistant pairs)
  const chatMessages = list.querySelectorAll('.chat-msg');
  if (chatMessages.length >= 10) {
    chatMessages[0].remove();
  }
  
  list.appendChild(msg);
  list.scrollTop = list.scrollHeight; // Auto scroll to bottom
}

// Helper function to escape HTML
function escapeHtml(text) {
  const map = {
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    "'": '&#039;'
  };
  return text.replace(/[&<>"']/g, m => map[m]);
}

function clearChat() {
  if (confirm('Clear chat history?')) {
    const list = document.getElementById('chatList');
    if (list) {
      list.innerHTML = '<div class="empty"><p>No messages yet. Start talking to Kiko!</p></div>';
    }
    sendMessage({ action: 'clear_chat' });
  }
}

// ========== LOAD LAST CAPTURED IMAGE ==========
function loadLastCapturedImage() {
  const gallery = document.getElementById('gallery');
  if (!gallery) return;
  
  const img = document.createElement('img');
  img.src = '/last_image.jpg?t=' + Date.now(); // Cache bust with timestamp
  img.alt = 'Captured image';
  
  // Remove existing image/placeholder
  const existing = gallery.querySelector('img');
  const empty = gallery.querySelector('.empty');
  if (existing) existing.remove();
  if (empty) empty.remove();
  
  gallery.appendChild(img);
  switchTab('gallery'); // Auto switch to gallery when image captured
}

// ========== GALLERY FUNCTIONS ==========
function addGalleryImage(imageUrl) {
  const gallery = document.getElementById('gallery');
  if (!gallery) return;
  
  // Remove "no images" placeholder if exists
  const empty = gallery.querySelector('.empty');
  if (empty) empty.remove();
  
  const item = document.createElement('div');
  item.className = 'gallery-item';
  item.innerHTML = `<img src="${escapeHtml(imageUrl)}" alt="Captured image">`;
  
  gallery.appendChild(item);
}

function clearGallery() {
  if (confirm('Clear gallery?')) {
    const gallery = document.getElementById('gallery');
    if (gallery) {
      gallery.innerHTML = '<div class="empty"><p>No images yet. Ask "What do you see?" to capture an image.</p></div>';
    }
    sendMessage({ action: 'clear_gallery' });
  }
}

// ========== TODO FUNCTIONS ==========
function clearTodos() {
  if (confirm('Clear all to-do lists?')) {
    const container = document.getElementById('todoLists');
    if (container) {
      container.innerHTML = '<div class="empty"><p>No to-do lists created yet.</p></div>';
    }
    sendMessage({ action: 'clear_todos' });
  }
}

// ========== TAB NAVIGATION ==========
function initTabNavigation() {
  const buttons = document.querySelectorAll('.tab-btn');
  const contents = document.querySelectorAll('.tab-content');
  
  buttons.forEach(btn => {
    btn.addEventListener('click', () => {
      const tab = btn.getAttribute('data-tab');
      switchTab(tab);
    });
  });
}

function switchTab(tabName) {
  state.currentTab = tabName;
  
  // Update buttons
  document.querySelectorAll('.tab-btn').forEach(btn => {
    if (btn.getAttribute('data-tab') === tabName) {
      btn.classList.add('active');
    } else {
      btn.classList.remove('active');
    }
  });
  
  // Update content
  document.querySelectorAll('.tab-content').forEach(content => {
    if (content.id === `tab-${tabName}`) {
      content.classList.add('active');
    } else {
      content.classList.remove('active');
    }
  });
}

// ========== UTILITY FUNCTIONS ==========
function sendMessage(data) {
  if (state.ws && state.ws.readyState === WebSocket.OPEN) {
    state.ws.send(JSON.stringify(data));
  }
}

function escapeHtml(text) {
  const div = document.createElement('div');
  div.textContent = text;
  return div.innerHTML;
}

function updateStatusIndicator(connected) {
  // Visual indicator for connection status
  // Could add a small indicator in the header
  console.log(connected ? '🟢 Connected' : '🔴 Disconnected');
}

function updateUI() {
  // Initial UI render - called on page load
  // Sets up all DOM elements with default values
  
  // Initialize transcription
  const transcription = document.getElementById('transcription');
  if (transcription) {
    transcription.textContent = 'Waiting for voice input...';
  }
  
  // Initialize alarm
  const alarm = document.getElementById('alarmCountdown');
  if (alarm) {
    alarm.textContent = 'No alarm set';
    alarm.className = 'alarm-inactive';
  }
  
  // Initialize chat
  const chat = document.getElementById('chatList');
  if (chat) {
    chat.innerHTML = '<div class="empty"><p>No messages yet. Start talking to Kiko!</p></div>';
  }
  
  // Initialize gallery
  const gallery = document.getElementById('gallery');
  if (gallery) {
    gallery.innerHTML = '<div class="empty"><p>No images yet. Ask "What do you see?" to capture an image.</p></div>';
  }
  
  // Initialize todos
  const todos = document.getElementById('todoLists');
  if (todos) {
    todos.innerHTML = '<div class="empty"><p>No to-do lists created yet.</p></div>';
  }
  
  // Initialize camera
  const camera = document.getElementById('cameraContainer');
  if (camera) {
    camera.innerHTML = '<div class="empty"><p>Camera not active. Enable surveillance mode to view.</p></div>';
  }
  
  // Initialize state
  const badge = document.getElementById('stateBadge');
  if (badge) {
    badge.textContent = 'Idle';
    badge.className = 'state-badge state-idle';
  }
}

// ========== PERIODIC UPDATES ==========
setInterval(() => {
  // Ping server to keep connection alive
  if (state.ws && state.ws.readyState === WebSocket.OPEN && state.lastState !== 'Surveillance') {
    sendMessage({ type: 'ping' });
  }
}, 30000); // Every 30 seconds

// ========== AUTO-REFRESH CHAT & GALLERY ==========
setInterval(() => {
  // Periodically fetch chat history and gallery to stay in sync
  if (state.ws && state.ws.readyState === WebSocket.OPEN) {
    sendMessage({ type: 'refresh_chat' });
    sendMessage({ type: 'refresh_gallery' });
  }
}, 5000); // Every 5 seconds

// ========== CLEAR DATA FUNCTIONS ==========

// Universal Confirmation Function
function showConfirm(message, onConfirm, buttonText = "Proceed") {
    const modal = document.getElementById('confirmModal');
    const msg = document.getElementById('modalMessage');
    const confirmBtn = document.getElementById('modalConfirm');
    const cancelBtn = document.getElementById('modalCancel');

    msg.textContent = message;
    confirmBtn.textContent = buttonText; // Set button text (e.g., "Cancel Alarm")
    modal.style.display = 'flex';

    // Handle Confirm
    confirmBtn.onclick = () => {
        modal.style.display = 'none';
        onConfirm();
    };

    // Handle Cancel
    cancelBtn.onclick = () => {
        modal.style.display = 'none';
    };
}

// Updated Clear Functions
function clearChat() {
    showConfirm("Do you want to delete the entire chat history?", async () => {
        await fetch('/clear_chat');
        document.getElementById('chatList').innerHTML = '<p class="empty">No messages yet.</p>';
    });
}

function clearGallery() {
    showConfirm("Do you want to delete all captured images?", async () => {
        await fetch('/clear_gallery');
        document.getElementById('gallery').innerHTML = '<p class="empty">No images yet.</p>';
    });
}

function clearTodos() {
    showConfirm("Do you want to clear all your to-do lists?", async () => {
        await fetch('/clear_todos');
        document.getElementById('todoLists').innerHTML = '<div class="empty"><p>No lists yet.</p></div>';
    });
}

// ========== EXPORT FOR TESTING ==========
if (typeof module !== 'undefined' && module.exports) {
  module.exports = { updateState, addChatMessage, switchTab };
}
