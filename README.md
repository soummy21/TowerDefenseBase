🏰 TowerDefenseBase

*A template for building 2D tower defense games built using C++ and SFML*

🎮 Project Overview

TowerDefenseBase is a base framework for developing a fully functional tower defense game. The system includes core mechanics such as enemy pathfinding, a tile-based level editor, and a custom AABB collision system that supports both circles and rectangles.

⚙️ Core Systems and Features

This project is designed to be modular and extendable, allowing additional mechanics like tower placement and upgrades to be built on top of the existing framework.

🔸 Pathfinding System

Uses a 2D grid-based navigation approach.

Enemies follow pre-determined paths defined in the level editor.

Supports dynamic movement adjustments based on obstacles.

🔸 Level Editor

Implements a tilemap system for painting levels.

Allows defining enemy paths interactively.

🔸 Custom Collision System

Uses an Axis-Aligned Bounding Box (AABB) approach.

Detects collisions between circular and rectangular objects.

Efficiently handles physics interactions within the game world.

🔸 Enemy System

Enemies spawn in waves and follow pathfinding rules.

Movement logic ensures smooth transitions between path nodes.

Integrates with the collision system for future combat mechanics.

🛠️ Installation & Running the Game

🔹 Download & Play

Go to the Releases page.

Download the latest TowerDefenseBase.zip file.

Extract the ZIP folder.

Double-click TowerDefenseBase.exe to run the game.

💡 Note: Windows might display a warning about an unknown publisher. Click More Info → Run Anyway to proceed.

🖥️ Building from Source

If you want to modify or expand the game, you can compile it from source.

🔹 Requirements

C++ Compiler (MSVC, MinGW, or Clang)

SFML 2.5.1 (or later)

Visual Studio 2022 (or any C++ IDE)

🔹 Steps

Clone the repository:

git clone https://github.com/yourrepo/TowerDefenseBase.git
cd TowerDefenseBase

Open TowerDefenseBase.sln in Visual Studio.

Set build configuration to Release.

Click Build → Build Solution (Ctrl + Shift + B).

Run the compiled .exe from x64/Release/.

