# 🐾 Kiko – The Desktop Pet Bot

*Kiko is a tiny AI-powered desktop companion that reacts to touch, shows emotions on a screen, and speaks back using a speaker.*

---
![Kiko](/images/kiko.jpg)


## ✨ Overview

**Kiko** is built using the **XIAO ESP32-S3 Sense** and combines audio, visuals, and touch interaction to create a small but expressive desktop pet.

It is designed to feel *alive* — blinking eyes, voice responses, mood lighting, and simple interactions.

---

## 🧠 Features

- 🔊 Voice output using I2S Audio (MAX98357A)
- 👀 OLED-based facial expressions
- 🌈 RGB LED emotion indicators
- ✋ Capacitive touch interaction
- ⚡ USB-powered (desktop friendly)
- 🤖 ESP32-S3 ready for AI / TinyML

---

## 🧰 Hardware Components

| Component | Description |
|---------|------------|
| XIAO ESP32-S3 Sense | Main controller |
| MAX98357A | I2S Audio Amplifier |
| Speaker (4Ω / 8Ω) | Audio output |
| OLED Display | SSD1306 / SH1106 |
| RGB LED | Visual feedback |
| Touch Pad | Capacitive touch input |

---

## 🔌 Full Connection Schematics

### 1️⃣ MAX98357A → XIAO ESP32-S3 Sense (Audio)

| MAX98357A Pin | XIAO ESP32-S3 Pin | Notes |
|--------------|------------------|------|
| Vin | VUSB / 5V | Provides maximum power |
| GND | GND | Ground |
| LRC | GPIO 7 | I2S Left/Right Clock |
| BCLK | GPIO 5 | I2S Bit Clock |
| DIN | GPIO 6 | I2S Data Input |
| GAIN | 100kΩ → GND | Sets gain to **15dB** |

**Why 15dB?**  
This is a balanced gain setting that keeps Kiko loud enough for a room while avoiding speaker distortion or clipping.

---

### 2️⃣ Speaker → MAX98357A

| Speaker | MAX98357A |
|-------|-----------|
| + | OUT+ |
| − | OUT− |

⚠️ Do **not** connect speaker pins to GND.

---

### 3️⃣ Interaction & Visuals

#### 🖥️ OLED Display (SSD1306 / SH1106)

| OLED Pin | XIAO ESP32-S3 |
|--------|---------------|
| SDA | GPIO 8 |
| SCL | GPIO 9 |
| VCC | 3.3V |
| GND | GND |

---

#### 🌈 RGB LED

| LED Channel | GPIO |
|------------|------|
| Red | GPIO 2 |
| Green | GPIO 1 |
| Blue | GPIO 3 |
| GND | GND |

---

#### ✋ Touch Pad

| Item | Connection |
|----|-----------|
| Touch Pin | GPIO 4 |
| Pad | Metal plate / copper foil |

Capacitive sensing works with a **single wire**, no resistor required.

---

## 🖼️ Circuit Schematic

![Kiko Circuit Schematic](/images/kikoSchematics.png)

---

## 🔄 System Flowchart

![Kiko System Flowchart](/images/kikoFlowchart.png)

---

## 🔄 System Dashboard

![Kiko System Flowchart](/images/dashboard.jpg)

## 🔁 Working Logic

1. Power On
2. Initialize OLED, Audio, Touch, RGB LED
3. Enter Idle Mode (blinking eyes, soft LED)
4. Touch Detected
5. Wake animation + LED color change
6. Voice response played via speaker
7. Return to Idle

---

## ⚡ Power Notes

- Audio amplifier powered from **VUSB / 5V**
- Logic and OLED powered from **3.3V**
- USB power is sufficient for normal operation

---

## 🚀 Future Improvements

- Camera-based emotion recognition
- On-device AI using ESP32-S3
- Microphone-based interaction
- Sleep / wake personality cycles
- Desktop presence detection

---

## 🧡 Philosophy

Kiko is not just a robot.

It’s a **presence** —  
a small companion that reacts, responds, and lives quietly on your desk.

---

## 📜 License

Open-source.  
Feel free to build, modify, and improve Kiko.
