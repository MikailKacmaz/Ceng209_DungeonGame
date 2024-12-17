# Ceng209_DungeonGame
# Dungeon Game

## **Gameplay**
Dungeon Game is a text-based adventure game where you explore rooms, collect items, and fight monsters to escape the dungeon. The goal is to survive and reach the final room while defeating enemies and managing your inventory.

### **Commands:**
- `look`: Inspect the current room.
- `move`: Move to the next room (if possible).
- `pick up`: Collect items in the room.
- `inventory`: View your inventory.
- `attack`: Fight monsters in the room.
- `save <filename>`: Save your progress to a file.
- `load <filename>`: Load a saved game.
- `List`: List all saved games.
- `exit`: Exit the game.

## **Code Structure**
- **`main.c`**: Contains the main game loop and handles player input.
- **Player and Room structs**:
  - `Player`: Tracks health and inventory (e.g., sword, spear).
  - `Room`: Stores room details like description, items, and monsters.
- **Key Functions**:
  - `look()`: Describes the current room.
  - `pickup()`: Adds items to the player's inventory.
  - `attack()`: Handles combat logic.
  - `move()`: Moves the player to the next room.
  - `saveGame()` / `loadGame()`: Save and load game state.
  - `listSavedGames()`: Displays saved files in the "dungeon" folder.

## **Game Logic**
1. The dungeon consists of 7 rooms, each with a unique description.
2. Some rooms contain items (sword, spear) or monsters (Goblin, Vampire).
3. Players must collect items to fight monsters.
4. Weapons break after defeating a monster.
5. If the player defeats all monsters and reaches the last room, they win the game.
6. The game can be saved and resumed later.

## **How to Compile**
A `Makefile` is provided for easy compilation. Run:
```bash
make
