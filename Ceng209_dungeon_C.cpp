#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <direct.h>    
#include <dirent.h>    


typedef struct {
    int health;
    int hasSword;
    int hasSpear;
} Player;


typedef struct {
    char* description;
    char* item;
    int hasMonster;
    char* monsterName; 
} Room;


void inventory(Player* player) {
    printf("Your inventory:\n");
    if (player->hasSword) {
        printf("Sword: You have it.\n");
    }
    if (player->hasSpear) {
        printf("Spear: You have it.\n");
    }
    if (!player->hasSword && !player->hasSpear) {
        printf("You have nothing in your inventory.\n");
    }
}


void look(Room* room) {
    printf("%s\n", room->description);
    if (room->item) {
        printf("There is a %s here.\n", room->item);
    }
    if (room->hasMonster) {
        printf("There is a %s here!\n", room->monsterName);
    }
}


void pickup(Player* player, Room* room) {
    if (room->item) {
        if (strcmp(room->item, "sword") == 0) {
            player->hasSword = 1;
            printf("Sword picked up!\n");
        } else if (strcmp(room->item, "spear") == 0) {
            player->hasSpear = 1;
            printf("Spear picked up!\n");
        }
        free(room->item); 
        room->item = NULL;
    } else {
        printf("There is nothing to pick up.\n");
    }
}


void attack(Player* player, Room* room) {
    if (room->hasMonster) {
        if (player->hasSword && room->monsterName != NULL && strcmp(room->monsterName, "Vampire") == 0) {
            printf("You attacked the Vampire and defeated it!\n");
            room->hasMonster = 0;
            room->monsterName = NULL; 
            printf("Your sword broke! The sword is now lost.\n");
            player->hasSword = 0;
        } else if (player->hasSpear && room->monsterName != NULL && strcmp(room->monsterName, "Vampire") == 0) {
            printf("You attacked the Vampire and defeated it!\n");
            room->hasMonster = 0;
            room->monsterName = NULL; 
            printf("Your spear broke! The spear is now lost.\n");
            player->hasSpear = 0;
        } else if (player->hasSword) {
            printf("You attacked the Goblin and defeated it!\n");
            room->hasMonster = 0;
            room->monsterName = NULL;
            printf("Your sword broke! The sword is now lost.\n");
            player->hasSword = 0;
        } else if (player->hasSpear) {
            printf("You attacked the Goblin and defeated it!\n");
            room->hasMonster = 0;
            room->monsterName = NULL;
            printf("Your spear broke! The spear is now lost.\n");
            player->hasSpear = 0;
        } else {
            printf("You have no weapon, the monster defeated you. Game over!\n");
            player->health = 0;
        }
    } else {
        printf("There is nothing to attack here.\n");
    }
}


void move(Room* rooms, Player* player, int* currentRoom) {
    if (rooms[*currentRoom].hasMonster) {
        printf("Monsters won't let you leave. You must fight to move forward!\n");
        return;
    }

    if (*currentRoom < 5) { 
        (*currentRoom)++;
        printf("Moving to the next room...\n");
    } else {
        
        printf("You have escaped the dungeon. Game over, you win!\n");
        player->health = 0;
    }
}


void saveGame(const char* filepath, Player* player, Room* rooms, int currentRoom) {
   
    #ifdef _WIN32
        _mkdir("dungeon"); 
    #else
        mkdir("dungeon", 0777); 
    #endif

   
    char fullPath[150];
    snprintf(fullPath, sizeof(fullPath), "dungeon/%s", filepath);

    FILE* file = fopen(fullPath, "wb");
    if (file == NULL) {
        printf("Game could not be saved!\n");
        return;
    }

   
    fwrite(player, sizeof(Player), 1, file);

    
    fwrite(&currentRoom, sizeof(int), 1, file);

   
    for (int i = 0; i < 7; i++) {
        size_t description_len = strlen(rooms[i].description) + 1;
        size_t item_len = rooms[i].item ? strlen(rooms[i].item) + 1 : 0;
        size_t monster_name_len = rooms[i].monsterName ? strlen(rooms[i].monsterName) + 1 : 0;

        fwrite(&description_len, sizeof(size_t), 1, file);
        fwrite(rooms[i].description, sizeof(char), description_len, file);

        fwrite(&item_len, sizeof(size_t), 1, file);
        if (rooms[i].item) {
            fwrite(rooms[i].item, sizeof(char), item_len, file);
        }

        fwrite(&rooms[i].hasMonster, sizeof(int), 1, file);

        fwrite(&monster_name_len, sizeof(size_t), 1, file);
        if (rooms[i].monsterName) {
            fwrite(rooms[i].monsterName, sizeof(char), monster_name_len, file);
        }
    }

    fclose(file);
    printf("Game successfully saved to the 'dungeon' folder: %s\n", fullPath);
}


void loadGame(const char* filepath, Player* player, Room* rooms, int* currentRoom) {
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Game could not be loaded!\n");
        return;
    }

   
    fread(player, sizeof(Player), 1, file);

   
    fread(currentRoom, sizeof(int), 1, file);

    
    for (int i = 0; i < 7; i++) {
        size_t description_len, item_len, monster_name_len;

        fread(&description_len, sizeof(size_t), 1, file);
        rooms[i].description = (char*)malloc(description_len);
        fread(rooms[i].description, sizeof(char), description_len, file);

        fread(&item_len, sizeof(size_t), 1, file);
        if (item_len > 0) {
            rooms[i].item = (char*)malloc(item_len);
            fread(rooms[i].item, sizeof(char), item_len, file);
        } else {
            rooms[i].item = NULL;
        }

        fread(&rooms[i].hasMonster, sizeof(int), 1, file);

        fread(&monster_name_len, sizeof(size_t), 1, file);
        if (monster_name_len > 0) {
            rooms[i].monsterName = (char*)malloc(monster_name_len);
            fread(rooms[i].monsterName, sizeof(char), monster_name_len, file);
        } else {
            rooms[i].monsterName = NULL;
        }
    }

    fclose(file);
    printf("Game successfully loaded!\n");
}


void listSavedGames() {
    const char* folder = "dungeon"; 
    DIR* dir = opendir(folder);   
    struct dirent* entry;

    if (dir == NULL) {
        printf("The 'dungeon' folder was not found or could not be accessed. No games might have been saved.\n");
        return;
    }

    printf("Saved games:\n");
    int gameCount = 0;

    while ((entry = readdir(dir)) != NULL) {
        
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("- %s\n", entry->d_name); 
            gameCount++;
        }
    }

    if (gameCount == 0) {
        printf("No saved games found.\n");
    }

    closedir(dir); 
}

int main() {
    Player player = { .health = 100, .hasSword = 0, .hasSpear = 0 };

    
    Room rooms[7];  

    const char* descriptions[7] = {
        "You are in Room 1. There is writing on the wall: 'This dungeon is full of monsters.'",
        "You are in Room 2.",
        "You are in Room 3.",
        "You are in Room 4. There is a portrait of a blood-sucking vampire on the wall.", 
        "You are in Room 5.",
        "You are in Room 6.", 
        "You are in Room 7. The dungeon ends here."
    };

    const char* items[7] = {
        NULL,
        "sword",
        NULL,
        NULL,
        "spear",
        NULL,
        NULL
    };

    int hasMonster[7] = { 0, 0, 1, 0, 0, 1, 0 }; 
    const char* monsters[7] = { NULL, NULL, "Goblin", NULL, NULL, "Vampire", NULL }; 

    for (int i = 0; i < 7; i++) {
        rooms[i].description = (char*)malloc(strlen(descriptions[i]) + 1);
        strcpy(rooms[i].description, descriptions[i]);

        if (items[i]) {
            rooms[i].item = (char*)malloc(strlen(items[i]) + 1);
            strcpy(rooms[i].item, items[i]);
        } else {
            rooms[i].item = NULL;
        }
        rooms[i].hasMonster = hasMonster[i];

        if (monsters[i] != NULL) {
            rooms[i].monsterName = strdup(monsters[i]);
        } else {
            rooms[i].monsterName = NULL;
        }
    }

    int currentRoom = 0;
    char command[50];
    char filepath[100];

    printf("Welcome to the Dungeon Game!\n");

    while (player.health > 0) {
        printf("\nEnter a command (look, move, pick up, attack, inventory, save <filepath>, load <filepath>, List, exit):\n");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "List") == 0) {
            listSavedGames(); 
        } else if (strcmp(command, "look") == 0) {
            look(&rooms[currentRoom]);
        } else if (strcmp(command, "pick up") == 0) {
            pickup(&player, &rooms[currentRoom]);
        } else if (strcmp(command, "inventory") == 0) {
            inventory(&player);
        } else if (strcmp(command, "attack") == 0) {
            attack(&player, &rooms[currentRoom]);
        } else if (strncmp(command, "save", 4) == 0) {
            sscanf(command, "save %s", filepath);
            saveGame(filepath, &player, rooms, currentRoom);
        } else if (strncmp(command, "load", 4) == 0) {
            sscanf(command, "load %s", filepath);
            loadGame(filepath, &player, rooms, &currentRoom);
        } else if (strcmp(command, "move") == 0) {
            move(rooms, &player, &currentRoom);
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting the game.\n");
            exit(0); 
        } else {
            printf("Undefined command!\n");
        }
    }

   
    if (player.health > 0 && currentRoom != 5) {
        printf("Game over. You lost!\n");
    }

    
    for (int i = 0; i < 7; i++) {
        free(rooms[i].description);
        if (rooms[i].item) free(rooms[i].item);
        if (rooms[i].monsterName) free(rooms[i].monsterName);
    }

    return 0;
}
