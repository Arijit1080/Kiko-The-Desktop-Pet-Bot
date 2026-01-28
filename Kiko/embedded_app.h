// Kiko AI Assistant - WebSocket Communication & UI Controller - EMBEDDED VERSION
const state = { currentTab: 'status', ws: null, wsAttempts: 0, maxWsAttempts: 10, wsRetryDelay: 3000, lastState: null, alarmInterval: null };
document.addEventListener('DOMContentLoaded', () => { initTabNavigation(); initWebSocket(); updateUI(); });
function initWebSocket() {
  const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
  const host = window.location.hostname;
  const wsUrl = protocol + '//' + host + ':81';
  try {
    state.ws = new WebSocket(wsUrl);
    state.ws.onopen = () => {
      console.log('Connected');
      state.wsAttempts = 0;
      updateStatusIndicator(true);
    };
    state.ws.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        handleMessage(data);
      } catch (e) {
        console.error('Failed to parse message:', e);
      }
    };
    state.ws.onerror = (error) => {
      console.error('WebSocket error:', error);
      updateStatusIndicator(false);
    };
    state.ws.onclose = () => {
      console.log('Disconnected');
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
    setTimeout(() => { initWebSocket(); }, state.wsRetryDelay);
  }
}
function handleMessage(data) {
  if (data.history && Array.isArray(data.history)) {
    const chatList = document.getElementById('chatList');
    if (chatList) {
      chatList.innerHTML = '';
      const filteredMessages = data.history.filter(msg => msg.role === 'user' || msg.role === 'assistant');
      if (filteredMessages.length === 0) {
        chatList.innerHTML = '<p class="empty">No messages yet.</p>';
      } else {
        const lastMessages = filteredMessages.slice(-10);
        lastMessages.forEach(msg => { addChatMessage(msg.role, msg.content); });
      }
    }
  }
  if (data.state) updateState(data.state);
  if (data.transcript !== undefined) updateTranscription(data.transcript);
  if (data.alarm_time !== undefined || data.alarm !== undefined || data.is_ringing !== undefined) {
    updateAlarm(data.alarm_time, data.alarm, data.is_ringing, data.remaining);
  }
  if (data.lists) updateTodoLists(data.lists);
  if (data.camera_mode !== undefined) updateCameraMode(data.camera_mode);
  if (data.message) addChatMessage(data.message.role, data.message.content);
  if (data.image_ready) loadLastCapturedImage();
  if (data.gallery && Array.isArray(data.gallery)) {
    const gallery = document.getElementById('gallery');
    if (gallery) {
      gallery.innerHTML = '';
      if (data.gallery.length === 0) {
        gallery.innerHTML = '<div class="empty"><p>No images yet.</p></div>';
      }
    }
  }
  if (data.image_url) addGalleryImage(data.image_url);
}
function updateState(newState) {
  if (state.lastState === newState) return;
  state.lastState = newState;
  const badge = document.getElementById('stateBadge');
  if (!badge) return;
  badge.textContent = newState;
  badge.className = 'state-badge';
  const stateMap = {
    'idle': 'state-idle',
    'listening': 'state-listening',
    'thinking': 'state-thinking',
    'speaking': 'state-speaking',
    'alarming': 'state-alarming',
    'surveillance': 'state-surveillance'
  };
  badge.classList.add(stateMap[newState.toLowerCase()] || 'state-idle');
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
  if (clearBtn) clearBtn.style.display = 'block';
  if (isRinging) {
    element.textContent = 'ALARM';
    element.classList.remove('alarm-inactive');
    element.classList.add('alarm-ringing');
    return;
  }
  element.classList.remove('alarm-inactive', 'alarm-ringing');
  element.classList.add('active');
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
    element.textContent = String(minutes).padStart(2, '0') + ':' + String(seconds).padStart(2, '0');
  }
  updateCountdown();
  state.alarmInterval = setInterval(updateCountdown, 1000);
}
function clearAlarm() {
  showConfirm('Cancel alarm?', async () => {
    try {
      const response = await fetch('/api/alarm/cancel', { method: 'POST' });
      if (response.ok) {
        const clearBtn = document.getElementById('clearAlarmBtn');
        if (clearBtn) clearBtn.style.display = 'none';
      }
    } catch (e) {
      console.error('Failed:', e);
    }
  }, 'Cancel');
}
function updateTodoLists(lists) {
  const container = document.getElementById('todoLists');
  if (!container) return;
  if (!lists || Object.keys(lists).length === 0) {
    container.innerHTML = '<div class="empty"><p>No lists yet.</p></div>';
    return;
  }
  let html = '';
  for (const [listName, items] of Object.entries(lists)) {
    html += '<div class="todo-list"><h3>' + escapeHtml(listName) + '</h3>';
    if (items && Object.keys(items).length > 0) {
      html += '<div>';
      for (const [itemName, quantity] of Object.entries(items)) {
        const qty = parseInt(quantity) || 0;
        html += '<div class="todo-item"><span>' + escapeHtml(itemName) + '</span><span class="qty">(' + qty + ')</span></div>';
      }
      html += '</div>';
    } else {
      html += '<p style="color: #999;">Empty</p>';
    }
    html += '</div>';
  }
  container.innerHTML = html;
}
function updateCameraMode(mode) {
  const container = document.getElementById('cameraContainer');
  if (!container) return;
  if (window.cameraRefreshInterval) {
    clearInterval(window.cameraRefreshInterval);
    window.cameraRefreshInterval = null;
  }
  if (mode === 'live' || mode === 'surveillance') {
    container.innerHTML = '<img id="cameraFeed" src="/stream" alt="Camera" style="width: 100%; border-radius: 8px; margin-top: 10px;">';
    container.classList.add('active');
    switchTab('camera');
  } else {
    container.classList.remove('active');
    container.innerHTML = '<div class="empty"><p>Camera inactive.</p></div>';
  }
}
function addChatMessage(role, content) {
  if (role !== 'user' && role !== 'assistant') return;
  const list = document.getElementById('chatList');
  if (!list) return;
  if (role === 'assistant') {
    try {
      const parsed = JSON.parse(content);
      if (parsed.tool_calls) return;
    } catch (e) {}
  }
  const empty = list.querySelector('.empty');
  if (empty) empty.remove();
  const msg = document.createElement('div');
  msg.className = 'chat-msg chat-' + (role === 'user' ? 'user' : 'ai');
  const icon = role === 'user' ? '👤' : '🤖';
  msg.innerHTML = '<span class="msg-icon">' + icon + '</span><span class="msg-text">' + escapeHtml(content) + '</span>';
  const chatMessages = list.querySelectorAll('.chat-msg');
  if (chatMessages.length >= 10) chatMessages[0].remove();
  list.appendChild(msg);
  list.scrollTop = list.scrollHeight;
}
function escapeHtml(text) {
  const map = { '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#039;' };
  return text.replace(/[&<>"']/g, m => map[m]);
}
function clearChat() {
  showConfirm('Clear chat?', async () => {
    await fetch('/clear_chat');
    document.getElementById('chatList').innerHTML = '<p class="empty">No messages.</p>';
  });
}
function loadLastCapturedImage() {
  const gallery = document.getElementById('gallery');
  if (!gallery) return;
  const img = document.createElement('img');
  img.src = '/last_image.jpg?t=' + Date.now();
  img.alt = 'Image';
  const existing = gallery.querySelector('img');
  const empty = gallery.querySelector('.empty');
  if (existing) existing.remove();
  if (empty) empty.remove();
  gallery.appendChild(img);
  switchTab('gallery');
}
function addGalleryImage(imageUrl) {
  const gallery = document.getElementById('gallery');
  if (!gallery) return;
  const empty = gallery.querySelector('.empty');
  if (empty) empty.remove();
  const item = document.createElement('div');
  item.className = 'gallery-item';
  item.innerHTML = '<img src="' + escapeHtml(imageUrl) + '" alt="Image">';
  gallery.appendChild(item);
}
function clearGallery() {
  showConfirm('Clear gallery?', async () => {
    await fetch('/clear_gallery');
    document.getElementById('gallery').innerHTML = '<p class="empty">No images.</p>';
  });
}
function clearTodos() {
  showConfirm('Clear todos?', async () => {
    await fetch('/clear_todos');
    document.getElementById('todoLists').innerHTML = '<div class="empty"><p>No lists.</p></div>';
  });
}
function initTabNavigation() {
  const buttons = document.querySelectorAll('.tab-btn');
  buttons.forEach(btn => {
    btn.addEventListener('click', () => {
      const tab = btn.getAttribute('data-tab');
      switchTab(tab);
    });
  });
}
function switchTab(tabName) {
  state.currentTab = tabName;
  document.querySelectorAll('.tab-btn').forEach(btn => {
    if (btn.getAttribute('data-tab') === tabName) {
      btn.classList.add('active');
    } else {
      btn.classList.remove('active');
    }
  });
  document.querySelectorAll('.tab-content').forEach(content => {
    if (content.id === 'tab-' + tabName) {
      content.classList.add('active');
    } else {
      content.classList.remove('active');
    }
  });
}
function sendMessage(data) {
  if (state.ws && state.ws.readyState === WebSocket.OPEN) {
    state.ws.send(JSON.stringify(data));
  }
}
function updateStatusIndicator(connected) {
  console.log(connected ? 'Connected' : 'Disconnected');
}
function updateUI() {
  const transcription = document.getElementById('transcription');
  if (transcription) transcription.textContent = 'Waiting...';
  const alarm = document.getElementById('alarmCountdown');
  if (alarm) { alarm.textContent = 'No alarm'; alarm.className = 'alarm-inactive'; }
  const chat = document.getElementById('chatList');
  if (chat) chat.innerHTML = '<p class="empty">No messages.</p>';
  const gallery = document.getElementById('gallery');
  if (gallery) gallery.innerHTML = '<p class="empty">No images.</p>';
  const todos = document.getElementById('todoLists');
  if (todos) todos.innerHTML = '<div class="empty"><p>No lists.</p></div>';
  const camera = document.getElementById('cameraContainer');
  if (camera) camera.innerHTML = '<div class="empty"><p>Inactive.</p></div>';
  const badge = document.getElementById('stateBadge');
  if (badge) { badge.textContent = 'Idle'; badge.className = 'state-badge state-idle'; }
}
setInterval(() => {
  if (state.ws && state.ws.readyState === WebSocket.OPEN && state.lastState !== 'Surveillance') {
    sendMessage({ type: 'ping' });
  }
}, 30000);
setInterval(() => {
  if (state.ws && state.ws.readyState === WebSocket.OPEN) {
    sendMessage({ type: 'refresh_chat' });
    sendMessage({ type: 'refresh_gallery' });
  }
}, 5000);
function showConfirm(message, onConfirm, buttonText = 'Proceed') {
  if (confirm(message)) onConfirm();
}
if (typeof module !== 'undefined' && module.exports) {
  module.exports = { updateState, addChatMessage, switchTab };
}
