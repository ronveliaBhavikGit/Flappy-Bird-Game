# 🐤 Flappy Bird Game on PSoC 6 (LVGL + FreeRTOS)

A fully functional **Flappy Bird** clone built on the **Infineon PSoC 6 Pioneer Kit** using **LVGL graphics**, **FreeRTOS tasks**, and **User button input & Capsense slider input**. 

This project demonstrates real-time multitasking, hardware abstraction, and UI animation — all running on a microcontroller!

---

## 🧩 Hardware Setup

**Board:**  
- PSoC 6 Pioneer Kit — `CY8CKIT-062S2-43012`

**Display:**  
- TFT Shield — `CY8CKIT-028-TFT (ST7789V Display Controller)`

**Input:**  
- User Button 2 (for jump action)
- CapSense Slider (for Game Screen Brightness)  

**Power:**  
- USB via KitProg3

---

## 🧠 Software Setup

| Component | Version / Tool |
|------------|----------------|
| ModusToolbox | 3.x |
| LVGL Library | v8.x |
| FreeRTOS | Integrated in ModusToolbox |
| Compiler | GCC ARM |
| Programming Interface | KitProg3 |
| Target | CY8C624ABZI-S2D44 |

---

## 🕹️ Game Features

- 30 FPS gameplay using **LVGL rendering**
- **Flappy physics simulation** (gravity + jump)
- **Pillar randomization** using `rand()`
- **Score counter** with collision detection
- **Start, Game Over, and Restart** screen management
- **CapSense / Button interrupt** for jump & reset
- Efficient **task-based architecture** (FreeRTOS)
- Dynamic screen refresh & UI updates

---

## ⚙️ System Architecture

### **Tasks Overview**
| Task | Description | Priority |
|------|--------------|-----------|
| `tft_task` | Handles LVGL rendering (`lv_task_handler`) | Highest |
| `game_task` | Runs Flappy Bird logic, updates objects | High |
| `capsense_task` | Reads CapSense button input | Medium |
| `led_task` | Optional debug LED blink | Low |

### **Game Video**
🎥 [Watch Gameplay Demo](images/demo.mp4)
