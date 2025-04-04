# 🏠 TowerDefenseBase
*A foundational tower defense game built with SFML*

---

## 🎮 Project Overview
TowerDefenseBase is a base framework for developing a fully functional tower defense game. The system includes core mechanics such as enemy pathfinding, a tile-based level editor, and a custom AABB collision system that supports both circles and rectangles.

## ⚙️ Core Features and Systems
This project is designed to be modular and extendable, allowing additional mechanics like tower placement and upgrades to be built on top of the existing framework.

### 🔸 Pathfinding System
- Uses a 2D grid-based navigation approach.
- Enemies follow pre-determined paths defined in the level editor.
- Supports dynamic movement adjustments based on obstacles.

### 🔸 Level Editor
- Implements a tilemap system for painting levels.
- Allows defining enemy paths interactively.

### 🔸 Custom Collision System
- Uses an Axis-Aligned Bounding Box (AABB) approach.
- Detects collisions between circular and rectangular objects.
- Efficiently handles physics interactions within the game world.

### 🔸 Enemy System
- Enemies spawn in waves and follow pathfinding rules.
- Movement logic ensures smooth transitions between path nodes.
- Integrates with the collision system for future combat mechanics.

---

## 🛠️ Installation & Running the Game

### 🔹 Download & Play
1. Go to the **[Releases](https://github.com/yourrepo/TowerDefenseBase/releases)** page.
2. Download the latest **TowerDefenseBase.zip** file.
3. Extract the ZIP folder.
4. Double-click `TowerDefenseBase.exe` to run the game.

💡 **Note:** Windows might display a warning about an unknown publisher. Click **More Info → Run Anyway** to proceed.

---

## 🖥️ Building from Source
If you want to modify or expand the game, you can compile it from source.

### 🔹 Requirements
- **C++ Compiler** (MSVC, MinGW, or Clang)
- **SFML 2.5.1** (or later)
- **Visual Studio 2022** (or any C++ IDE)

### 🔹 Steps
1. Clone the repository:
   ```sh
   git clone https://github.com/yourrepo/TowerDefenseBase.git
   cd TowerDefenseBase
   ```
2. Open `TowerDefenseBase.sln` in Visual Studio.
3. Set **build configuration** to `Release`.
4. Click **Build → Build Solution (Ctrl + Shift + B)**.
5. Run the compiled `.exe` from `x64/Release/`.

---
