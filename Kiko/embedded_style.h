/* Kiko AI Assistant - Futuristic Dark Theme CSS - EMBEDDED VERSION */
:root {
  --bg-dark: #0a0e27;
  --bg-card: #1e2139;
  --primary: #00d4ff;
  --primary-light: #4fc1ff;
  --accent: #667eea;
  --text-main: #e8eaed;
  --text-dim: #999999;
  --state-idle: #667eea;
  --state-listening: #f5576c;
  --state-thinking: #ffa500;
  --state-speaking: #43e97b;
  --state-alarm: #fa709a;
  --state-surveillance: #ff3333;
  --border-glow: rgba(0, 212, 255, 0.2);
  --border-glow-strong: rgba(0, 212, 255, 0.4);
}
* { margin: 0; padding: 0; box-sizing: border-box; }
html { font-size: 16px; }
body {
  font-family: system-ui, -apple-system, sans-serif;
  background: linear-gradient(135deg, var(--bg-dark) 0%, #0f1432 100%);
  color: var(--text-main);
  line-height: 1.6;
  min-height: 100vh;
  overflow-x: hidden;
}
.container { max-width: 1200px; margin: 0 auto; padding: 20px; }
.header {
  background: linear-gradient(135deg, rgba(0, 212, 255, 0.12) 0%, rgba(79, 193, 255, 0.08) 100%);
  padding: 40px 30px;
  border-radius: 16px;
  border: 1px solid var(--border-glow);
  margin-bottom: 30px;
  text-align: center;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3), inset 0 1px 1px rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(10px);
}
.header h1 {
  font-size: clamp(2em, 5vw, 3em);
  color: var(--primary);
  margin-bottom: 8px;
  font-weight: 700;
  letter-spacing: 2px;
  text-shadow: 0 0 20px rgba(0, 212, 255, 0.4);
}
.header p { color: var(--primary-light); font-size: 0.95em; letter-spacing: 1px; }
.mic-icon { display: inline-block; font-size: 1.3em; margin-right: 12px; animation: float 3s ease-in-out infinite; }
@keyframes float { 0%, 100% { transform: translateY(0px); } 50% { transform: translateY(-8px); } }
.tabs {
  display: flex;
  gap: 8px;
  margin-bottom: 30px;
  border-bottom: 2px solid var(--border-glow);
  padding-bottom: 12px;
  overflow-x: auto;
  scroll-behavior: smooth;
}
.tabs::-webkit-scrollbar { height: 4px; }
.tabs::-webkit-scrollbar-track { background: transparent; }
.tabs::-webkit-scrollbar-thumb { background: var(--primary); border-radius: 2px; }
.tab-btn {
  background: transparent;
  color: var(--text-dim);
  border: none;
  padding: 12px 24px;
  cursor: pointer;
  font-size: 0.95em;
  font-weight: 500;
  border-bottom: 3px solid transparent;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  white-space: nowrap;
  position: relative;
}
.tab-btn:hover { color: var(--primary); }
.tab-btn.active { color: var(--primary); border-bottom-color: var(--primary); }
.tab-btn.active::after {
  content: '';
  position: absolute;
  bottom: -12px;
  left: 0;
  right: 0;
  height: 1px;
  background: radial-gradient(ellipse at center, var(--primary) 0%, transparent 70%);
  filter: blur(1px);
}
.tab-content { display: none; animation: fadeInDown 0.4s cubic-bezier(0.4, 0, 0.2, 1); }
.tab-content.active { display: block; }
@keyframes fadeInDown { from { opacity: 0; transform: translateY(10px); } to { opacity: 1; transform: translateY(0); } }
.card {
  background: rgba(30, 33, 57, 0.8);
  border: 1px solid var(--border-glow);
  border-radius: 14px;
  padding: 28px;
  margin-bottom: 25px;
  box-shadow: 0 10px 40px rgba(0, 0, 0, 0.3), inset 0 1px 1px rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}
.card:hover {
  border-color: var(--border-glow-strong);
  box-shadow: 0 15px 50px rgba(0, 212, 255, 0.15), inset 0 1px 1px rgba(255, 255, 255, 0.05);
}
.card h2 {
  color: var(--primary);
  font-size: 1.35em;
  margin-bottom: 18px;
  display: flex;
  align-items: center;
  gap: 12px;
  font-weight: 600;
  letter-spacing: 0.5px;
}
.card h2::before {
  content: '';
  display: inline-block;
  width: 4px;
  height: 1.3em;
  background: linear-gradient(180deg, var(--primary) 0%, var(--primary-light) 100%);
  border-radius: 2px;
}
.state-badge {
  display: inline-block;
  padding: 16px 36px;
  border-radius: 30px;
  font-size: clamp(1em, 2.5vw, 1.2em);
  font-weight: 700;
  text-align: center;
  margin: 20px auto;
  display: block;
  letter-spacing: 1px;
  box-shadow: 0 8px 25px rgba(0, 0, 0, 0.3);
  transition: all 0.3s ease;
  text-transform: uppercase;
}
.state-idle {
  background: linear-gradient(135deg, var(--state-idle) 0%, #8b9ef6 100%);
  color: white;
  box-shadow: 0 8px 25px rgba(102, 126, 234, 0.3);
}
.state-listening {
  background: linear-gradient(135deg, var(--state-listening) 0%, #ff7a8e 100%);
  color: white;
  box-shadow: 0 0 25px rgba(245, 87, 108, 0.5);
  animation: pulse-listening 1s ease-in-out infinite;
}
@keyframes pulse-listening { 0%, 100% { box-shadow: 0 0 25px rgba(245, 87, 108, 0.5); } 50% { box-shadow: 0 0 50px rgba(245, 87, 108, 0.8); } }
.state-thinking {
  background: linear-gradient(135deg, var(--state-thinking) 0%, #ffb84d 100%);
  color: #000;
  box-shadow: 0 0 25px rgba(255, 165, 0, 0.5);
  animation: pulse-thinking 1.5s ease-in-out infinite;
}
@keyframes pulse-thinking { 0%, 100% { box-shadow: 0 0 25px rgba(255, 165, 0, 0.5); } 50% { box-shadow: 0 0 50px rgba(255, 165, 0, 0.8); } }
.state-speaking {
  background: linear-gradient(135deg, var(--state-speaking) 0%, #61f5a6 100%);
  color: #000;
  box-shadow: 0 8px 25px rgba(67, 233, 123, 0.4);
  animation: pulse-speaking 0.8s ease-in-out infinite;
}
@keyframes pulse-speaking { 0%, 100% { box-shadow: 0 8px 25px rgba(67, 233, 123, 0.4); } 50% { box-shadow: 0 8px 40px rgba(67, 233, 123, 0.7); } }
.state-alarming {
  background: linear-gradient(135deg, var(--state-alarm) 0%, #ff5a82 100%);
  color: white;
  box-shadow: 0 0 25px rgba(250, 112, 154, 0.6);
  animation: pulse-alarm 0.5s ease-in-out infinite;
  font-size: 1.4em;
  font-weight: 800;
}
@keyframes pulse-alarm { 0%, 100% { box-shadow: 0 0 25px rgba(250, 112, 154, 0.6); transform: scale(1); } 50% { box-shadow: 0 0 50px rgba(250, 112, 154, 1); transform: scale(1.02); } }
@keyframes alarm-ring { 0%, 100% { box-shadow: 0 0 40px rgba(255, 59, 48, 0.6), inset 0 0 20px rgba(255, 59, 48, 0.2); transform: scale(1); } 50% { box-shadow: 0 0 60px rgba(255, 59, 48, 0.9), inset 0 0 30px rgba(255, 59, 48, 0.4); transform: scale(1.03); } }
.state-surveillance {
  background: linear-gradient(135deg, var(--state-surveillance) 0%, #ff6666 100%);
  color: white;
  box-shadow: 0 0 30px rgba(255, 51, 51, 0.6);
  border: 2px solid var(--state-surveillance);
}
#transcription {
  background: rgba(0, 212, 255, 0.08);
  padding: 18px;
  border-radius: 10px;
  min-height: 50px;
  border: 2px dashed var(--primary);
  font-style: italic;
  color: var(--primary-light);
  font-size: 0.95em;
  word-wrap: break-word;
  transition: all 0.3s ease;
}
#transcription.active { background: rgba(0, 212, 255, 0.15); border-style: solid; border-color: var(--primary); box-shadow: inset 0 0 15px rgba(0, 212, 255, 0.1); }
#chatList {
  max-height: 450px;
  overflow-y: auto;
  border: 1px solid var(--border-glow);
  border-radius: 10px;
  padding: 15px;
  background: rgba(0, 0, 0, 0.2);
}
#chatList::-webkit-scrollbar { width: 6px; }
#chatList::-webkit-scrollbar-track { background: rgba(0, 212, 255, 0.05); border-radius: 3px; }
#chatList::-webkit-scrollbar-thumb { background: rgba(0, 212, 255, 0.3); border-radius: 3px; }
#chatList::-webkit-scrollbar-thumb:hover { background: rgba(0, 212, 255, 0.5); }
.chat-msg { padding: 12px 16px; margin-bottom: 12px; border-radius: 10px; word-wrap: break-word; animation: slideIn 0.3s ease; display: flex; align-items: flex-start; gap: 10px; }
@keyframes slideIn { from { opacity: 0; transform: translateX(-10px); } to { opacity: 1; transform: translateX(0); } }
.msg-icon { font-size: 1.5em; min-width: 24px; text-align: center; flex-shrink: 0; }
.msg-text { flex: 1; word-wrap: break-word; }
.chat-user { background: linear-gradient(135deg, rgba(102, 126, 234, 0.2) 0%, rgba(118, 75, 162, 0.1) 100%); border-left: 3px solid var(--accent); justify-content: flex-end; flex-direction: row-reverse; }
.chat-user .msg-icon { color: var(--accent); }
.chat-ai { background: linear-gradient(135deg, rgba(0, 212, 255, 0.15) 0%, rgba(79, 193, 255, 0.08) 100%); border-left: 3px solid var(--primary); }
.chat-ai .msg-icon { color: var(--primary); }
#alarmCountdown {
  font-size: clamp(2em, 6vw, 3em);
  font-weight: 900;
  color: var(--state-alarm);
  text-align: center;
  font-family: 'Courier New', monospace;
  padding: 24px;
  background: linear-gradient(135deg, rgba(255, 107, 107, 0.12) 0%, rgba(255, 193, 7, 0.08) 100%);
  border-radius: 12px;
  border: 2px solid rgba(255, 107, 107, 0.3);
  letter-spacing: 2px;
  transition: all 0.3s ease;
}
#alarmCountdown.active { box-shadow: 0 0 30px rgba(250, 112, 154, 0.4), inset 0 0 15px rgba(255, 107, 107, 0.1); animation: pulse-alarm 0.6s ease-in-out infinite; }
#alarmCountdown.alarm-ringing { background: linear-gradient(135deg, rgba(255, 59, 48, 0.25) 0%, rgba(255, 87, 34, 0.15) 100%); border-color: rgba(255, 59, 48, 0.6); box-shadow: 0 0 40px rgba(255, 59, 48, 0.6), inset 0 0 20px rgba(255, 59, 48, 0.2); animation: alarm-ring 0.4s ease-in-out infinite; }
#alarmCountdown.alarm-about-to-ring { background: linear-gradient(135deg, rgba(255, 152, 0, 0.2) 0%, rgba(255, 193, 7, 0.15) 100%); border-color: rgba(255, 193, 7, 0.5); animation: pulse-alarm 0.5s ease-in-out infinite; }
.alarm-inactive { color: var(--state-speaking); font-size: 1.1em; animation: none; box-shadow: none; }
.todo-list {
  background: linear-gradient(135deg, rgba(0, 212, 255, 0.08) 0%, rgba(79, 193, 255, 0.05) 100%);
  padding: 18px;
  border-radius: 10px;
  border-left: 4px solid var(--primary);
  margin-bottom: 18px;
  box-shadow: inset 0 1px 1px rgba(255, 255, 255, 0.05);
  transition: all 0.3s ease;
}
.todo-list:hover { background: linear-gradient(135deg, rgba(0, 212, 255, 0.12) 0%, rgba(79, 193, 255, 0.08) 100%); border-left-color: var(--primary-light); }
.todo-list h3 { color: var(--primary); font-size: 1.05em; margin-bottom: 12px; text-transform: capitalize; font-weight: 600; }
.todo-item { padding: 10px 12px; background: rgba(255, 255, 255, 0.03); border-radius: 6px; margin-bottom: 6px; display: flex; justify-content: space-between; align-items: center; font-size: 0.95em; transition: all 0.2s ease; border-left: 2px solid var(--primary-light); }
.todo-item:hover { background: rgba(255, 255, 255, 0.06); padding-left: 14px; }
.todo-item .qty { background: linear-gradient(135deg, var(--primary) 0%, var(--primary-light) 100%); color: #000; padding: 3px 10px; border-radius: 12px; font-weight: 700; font-size: 0.8em; min-width: 35px; text-align: center; box-shadow: 0 4px 12px rgba(0, 212, 255, 0.2); }
#gallery { display: grid; grid-template-columns: repeat(auto-fill, minmax(250px, 1fr)); gap: 16px; max-height: 550px; overflow-y: auto; }
#gallery::-webkit-scrollbar { width: 6px; }
#gallery::-webkit-scrollbar-track { background: rgba(0, 212, 255, 0.05); }
#gallery::-webkit-scrollbar-thumb { background: rgba(0, 212, 255, 0.3); border-radius: 3px; }
.gallery-item { border-radius: 12px; overflow: hidden; border: 2px solid var(--border-glow); transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1); box-shadow: 0 8px 20px rgba(0, 0, 0, 0.3); }
.gallery-item:hover { border-color: var(--primary); box-shadow: 0 12px 40px rgba(0, 212, 255, 0.2); transform: translateY(-4px); }
.gallery-item img { width: 100%; height: auto; display: block; transition: transform 0.3s ease; }
.gallery-item:hover img { transform: scale(1.05); }
#cameraContainer { position: relative; border-radius: 12px; overflow: hidden; border: 2px solid var(--border-glow); box-shadow: 0 10px 40px rgba(0, 0, 0, 0.3); }
#cameraContainer.active { border-color: var(--state-surveillance); box-shadow: 0 0 30px rgba(255, 51, 51, 0.3); }
#cameraFeed { width: 100%; height: auto; display: block; border-radius: 10px; border: 1px solid var(--border-glow); }
.btn {
  background: rgba(0, 212, 255, 0.15);
  color: var(--primary);
  border: 1.5px solid var(--primary);
  padding: 12px 28px;
  border-radius: 8px;
  cursor: pointer;
  font-size: 0.9em;
  font-weight: 600;
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  margin-top: 18px;
  letter-spacing: 0.5px;
}
.btn:hover { background: rgba(0, 212, 255, 0.25); box-shadow: 0 6px 20px rgba(0, 212, 255, 0.2); transform: translateY(-2px); }
.btn:active { transform: translateY(0); box-shadow: 0 3px 10px rgba(0, 212, 255, 0.15); }
.btn-danger { background: rgba(255, 107, 107, 0.15); color: #ff6b6b; border-color: #ff6b6b; }
.btn-danger:hover { background: rgba(255, 107, 107, 0.25); box-shadow: 0 6px 20px rgba(255, 107, 107, 0.2); }
.empty { text-align: center; padding: 50px 20px; color: var(--text-dim); }
.empty p { margin: 10px 0; font-size: 0.95em; }
@media (max-width: 768px) {
  .container { padding: 15px; }
  .header { padding: 25px 20px; margin-bottom: 20px; }
  .header h1 { font-size: 1.8em; }
  .card { padding: 18px; margin-bottom: 18px; }
  .card h2 { font-size: 1.15em; margin-bottom: 12px; }
  .tabs { gap: 4px; margin-bottom: 20px; padding-bottom: 8px; }
  .tab-btn { padding: 10px 16px; font-size: 0.85em; }
  .state-badge { padding: 12px 24px; font-size: 1em; }
  #chatList { max-height: 300px; }
  #gallery { grid-template-columns: 1fr; max-height: 400px; }
  #alarmCountdown { font-size: 2em; padding: 18px; }
  .btn { padding: 10px 20px; font-size: 0.85em; }
}
@media (max-width: 480px) {
  html { font-size: 14px; }
  .container { padding: 12px; }
  .header { padding: 20px 15px; }
  .header h1 { font-size: 1.5em; }
  .card { padding: 15px; }
  .tabs { gap: 2px; }
  .tab-btn { padding: 8px 12px; font-size: 0.75em; }
  #transcription { font-size: 0.85em; }
  .state-badge { padding: 10px 18px; font-size: 0.9em; }
}
@media (prefers-reduced-motion: reduce) {
  *, *::before, *::after { animation-duration: 0.01ms !important; animation-iteration-count: 1 !important; transition-duration: 0.01ms !important; }
}
