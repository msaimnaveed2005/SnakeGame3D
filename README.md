# SnakeGame3D

SnakeGame3D is a 3D remake of the classic Snake game developed in **C++** using **raylib**.  
It combines traditional snake gameplay with a 3D board, menu-driven interaction, sound integration, score saving, and multiple visual display options.



![Demo](https://raw.githubusercontent.com/msaimnaveed2005/SnakeGame3D/main/3DSnakeOOP2026-04-2116-59-46-ezgif.com-video-to-gif-converter.gif)



## Features

- 3D snake movement on a grid-based board
- Snake grows after eating food
- Score tracking system
- Wall collision detection
- Self-collision detection
- Restart system after game over
- Player name input
- Scoreboard with player name, score, and date
- Menu system with multiple options
- Visual Style selection:
  - Plain
  - Modern
- Camera Orientation selection:
  - Diagonal View
  - Top View
- Background music
- Sound effects for:
  - eating food
  - game over
  - new high score
- High score achievement banner
- Persistent score saving using file handling

## Technologies Used

- **C++**
- **raylib**
- Object-Oriented Programming
- File Handling
- Audio Integration
- 3D Rendering Concepts

## Project Structure

- `main.cpp`  
  Handles game loop, menu system, scoreboard, player input, audio, and screen transitions.

- `Snake.h / Snake.cpp`  
  Contains snake logic such as movement, growth, collision detection, and rendering.

- `Food.h / Food.cpp`  
  Contains food spawning, animation, reset logic, and rendering.

## Gameplay Flow

1. Launch the game
2. Enter player name if starting for the first time
3. Choose visual style and orientation from menu
4. Start the game
5. Eat food to increase score and grow the snake
6. Avoid walls and your own body
7. View scoreboard from menu
8. Restart after game over or return to menu

## Controls

### Menu
- **Up / Down** → Navigate menu
- **Left / Right** → Change style or orientation
- **Enter** → Select option
- **Backspace** → Return from scoreboard

### Gameplay
- **Arrow Keys** → Move snake
- **R** → Restart after game over
- **M** → Return to menu

## Audio Files Required

Place these files in the executable/output directory:

- `eat.wav`
- `gameover.wav`
- `win.wav`
- `bgmusic.mp3`

If a sound file is missing, the game will display a warning message on screen.

## Score Saving

Scores are stored in `scores.txt` with:

- player name
- score
- date

This allows the scoreboard to show ranked entries.

## Learning Outcomes

This project helped in understanding:

- Object-Oriented Programming in C++
- Class-based game design
- 3D coordinate mapping
- Collision handling
- Real-time game loops
- Menu and state management
- File-based score persistence
- Audio handling in raylib
- Visual enhancement techniques in a 3D environment

## Future Improvements

Possible future upgrades:

- particle effects
- smoother camera transitions
- animated snake movement
- pause system
- difficulty levels
- textured environment
- better UI design
- mouse-supported menus

## Author

Developed by **msaimnaveed2005**
