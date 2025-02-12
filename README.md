# 🏆 Maze Solver Bot - Technoxian 2023 Winner

## 🚀 Overview

This project is a **maze solver bot** that utilizes the **flood fill algorithm** to navigate and solve a maze autonomously. The bot was developed for the **Technoxian 2023 Maze Solver Competition**, where our team 'HAWKS' emerged as the **winner**. 🎉

## ✨ Features

- Uses **Arduino Nano** as the main controller. 🤖
- Employs an **array of 5 IR sensors** for line detection. 🔍
- Utilizes **TB6612FNG motor driver** to control movement. ⚙️
- Implements the **flood fill algorithm** for pathfinding. 🗺️
- Stores and updates maze values in **EEPROM**. 💾
- Equipped with **two 600 RPM micro metal gear motors** for precise movement. 🔄
- Supports **OLED display for debugging and monitoring**. 📟

## 🔧 Components Used

- **Arduino Nano** (Microcontroller) 🎛️
- **TB6612FNG** (Motor driver) ⚡
- **5 IR sensors** (Line detection and obstacle detection) 🛑
- **2 x 600 RPM micro metal gear motors** 🚗
- **OLED Display** (Optional for debugging) 📺

## 🧠 Software & Algorithm

### **Flood Fill Algorithm**

The bot utilizes the **flood fill algorithm** to navigate through the maze efficiently. The key steps include:

1. **Scanning walls** at the current cell using IR sensors. 🔎
2. **Updating the flood fill array** based on available paths. 🔢
3. **Choosing the optimal path** with the lowest flood value. ✅
4. **Navigating to the target cell** while avoiding obstacles. 🚶‍♂️
5. **Updating the maze map** and saving progress in EEPROM. 📂

### **Code Structure**

The codebase is divided into multiple files for modularity:

- **`micromouse.ino`** - Main program logic (setup and loop functions). 🎯
- **`motors.ino`** - Motor control logic. 🏎️
- **`debug.ino`** - Debugging utilities and serial monitoring. 🛠️
- **`floodfill.cpp / floodfill.h`** - Flood fill algorithm implementation. 🔄
- **`globals.h`** - Global variables and constants. 📌
- **`menu.cpp / menu.h`** - User interface and display functions. 📊
- **`save_maze.cpp / save_maze.h`** - EEPROM functions for saving and loading maze data. 💾

## 🛠️ How to Use

### **Setup Instructions**

1. **Clone the repository**:
   ```bash
   git clone https://github.com/PRASANTH9819/MAZE-SOLVER
   ```
2. **Upload the code to Arduino Nano** using the Arduino IDE. 🖥️
3. **Assemble the hardware** as per the component list. 🔧
4. **Power on the bot** and place it at the starting position. ⚡
5. **Calibrate the sensors** and start the bot. 🔄
6. The bot will **scan, navigate, and solve the maze** automatically. 🎯

## 🔮 Future Improvements

- **Implement machine learning** to optimize pathfinding. 🤖
- **Enhance sensor accuracy** using better calibration techniques. 📈
- **Develop a wireless debugging system** to monitor bot performance in real-time. 📡

## 🙌 Acknowledgments

A big thanks to our **team members** and **Technoxian 2023 organizers** for providing a great platform to showcase our work! 🎊

---

### 🔗 **Connect with Us**

For any queries or collaborations, feel free to contact us or contribute to the repository! 💬

