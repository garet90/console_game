// ConsoleGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
using namespace std;

const int CHUNK_COUNT = 8000;
const int CHUNK_SPREAD = 1000;
const int CHUNK_DIE = 1;
const int TREE_COUNT = 2000;
const int MAP_WIDTH = 300;
const int MAP_HEIGHT = 150;
const int FRAME_WIDTH = 41;
const int FRAME_HEIGHT = 21;
const int MAP_SEED = 12345678910;
const int BASE_WIDTH = 61;
const int BASE_HEIGHT = 31;
vector<string> map;
long int chunkSpreadChance = 0;
long int frameLoc = ((MAP_WIDTH / 2) - (FRAME_WIDTH / 2)) + (((MAP_HEIGHT / 2) * MAP_WIDTH) - ((FRAME_HEIGHT / 2) * MAP_WIDTH));
long int worldFrameLoc;
int health = 10;
int maxHealth = 10;
int stamina = 10;
int maxStamina = 10;
int hunger = 20;
int maxHunger = 20;
int weight = 0;
int weightCapacity = 100;
int inBase = -1;
int timeHours = 12;
int timeMinutes = 0;
int timeDay = 1;
int timeSeason = 1; // 0: spring, 1: summer, 2: fall, 3: winter
int inMenu = 0;
int selectedContainer;
int walkEase = 1;
int mineBuff = 0;
int mineEase = 1;
int axeBuff = 0;
int axeEase = 1;
string subCommand;
string onBlock;
vector<string> inventory;
vector<vector<string>> bases;
vector<vector<string>> containers;
string outputText = "";

int getItemDetails(string itemTGD, string requestTGD) {
	if (itemTGD == "raw meat") {
		if (requestTGD == "weight") return 5;
		if (requestTGD == "nutrition") return 5;
		if (requestTGD == "healing") return 0;
		if (requestTGD == "edible") return 1;
		if (requestTGD == "cookable") return 1;
	}
	if (itemTGD == "cooked meat") {
		if (requestTGD == "weight") return 5;
		if (requestTGD == "nutrition") return 15;
		if (requestTGD == "healing") return 0;
		if (requestTGD == "edible") return 1;
	}
	if (itemTGD == "raw hide") {
		if (requestTGD == "weight") return 3;
	}
	else if (itemTGD == "raw fish") {
		if (requestTGD == "weight") return 3;
		if (requestTGD == "nutrition") return 5;
		if (requestTGD == "healing") return 1;
		if (requestTGD == "edible") return 1;
		if (requestTGD == "cookable") return 1;
	}
	else if (itemTGD == "cooked fish") {
		if (requestTGD == "weight") return 3;
		if (requestTGD == "nutrition") return 15;
		if (requestTGD == "healing") return 5;
		if (requestTGD == "edible") return 1;
	}
	else if (itemTGD == "wood") {
		if (requestTGD == "weight") return 3;
		if (requestTGD == "hardness") return 4;
		if (requestTGD == "drop") return 4;
	}
	else if (itemTGD == "stone") {
		if (requestTGD == "weight") return 5;
		if (requestTGD == "hardness") return 4;
		if (requestTGD == "drop") return 4;
	}
	else if (itemTGD == "coal") {
		if (requestTGD == "weight") return 5;
		if (requestTGD == "hardness") return 4;
		if (requestTGD == "drop") return 4;
	}
	else if (itemTGD == "iron") {
		if (requestTGD == "weight") return 10;
		if (requestTGD == "hardness") return 6;
		if (requestTGD == "drop") return 3;
	}
	else if (itemTGD == "steel") {
		if (requestTGD == "weight") return 20;
		if (requestTGD == "hardness") return 8;
		if (requestTGD == "drop") return 2;
	}
	else if (itemTGD == "small crafting table") {
		if (requestTGD == "weight") return 10;
		if (requestTGD == "placeable") return 1;
	}
	else if (itemTGD == "small furnace") {
		if (requestTGD == "weight") return 10;
		if (requestTGD == "placeable") return 1;
	}
	else if (itemTGD == "small storage") {
		if (requestTGD == "weight") return 10;
		if (requestTGD == "placeable") return 1;
	}
	else if (itemTGD == "small hide backpack") {
		if (requestTGD == "weight") return 3;
		if (requestTGD == "weightbuff") return 50;
	}
	else if (itemTGD == "stone pickaxe") {
		if (requestTGD == "minebuff") return 2;
		if (requestTGD == "mineease") return 1.5;
		if (requestTGD == "weight") return 5;
		if (requestTGD == "durability") return 50;
	}
	else if (itemTGD == "stone axe") {
		if (requestTGD == "axebuff") return 2;
		if (requestTGD == "axeease") return 1.5;
		if (requestTGD == "weight") return 5;
		if (requestTGD == "durability") return 50;
	}
	else if (itemTGD == "stone sword") {
		if (requestTGD == "damage") return 3;
		if (requestTGD == "weight") return 5;
		if (requestTGD == "durability") return 500;
	}
	else if (itemTGD == "leather armor") {
		if (requestTGD == "healthbuff") return 5;
		if (requestTGD == "weight") return 10;
		if (requestTGD == "durability") return 500;
	}
	else if (itemTGD == "leather boots") {
		if (requestTGD == "staminabuff") return 5;
		if (requestTGD == "walkease") return 1.5;
		if (requestTGD == "weight") return 5;
		if (requestTGD == "durability") return 500;
	}
	return 0;
}

void moveOutOfBounds() {
	int rout = rand() % 4;
	if (rout == 0) {
		outputText = "Strange winds push you back, you cannot move this direction.";
	}
	else if (rout == 1) {
		outputText = "You decide you don't really want to go this direction.";
	}
	else if (rout == 2) {
		outputText = "You have a feeling you shouldn't go this way, so you don't";
	}
	else if (rout == 3) {
		outputText = "You broke your ankle, so you didn't move.";
		health--;
	}
}

void dropHunger(int amnt) {
	for (int i = 0; i < amnt; i++) {
		if (hunger > 0) {
			hunger--;
		}
		else {
			stamina--;
		}
	}
}

void removeItemFromInventory(string item, int count = 1) {
	for (int i = 0; i < count; i++) {
		bool removedItem = false;
		for (int ii = 0; ii < inventory.size(); ii++) {
			if (inventory[ii] == item && removedItem == false) {
				inventory.erase(inventory.begin() + ii);
				removedItem = true;
				weight -= getItemDetails(item,"weight");
			}
		}
	}
}

void addItemToInventory(string item, int count = 1) {
	for (int i = 0; i < count; i++) {
		inventory.push_back(item);
		weight += getItemDetails(item, "weight");
		if (100 + getItemDetails(item, "weightbuff") > weightCapacity) {
			weightCapacity = 100 + getItemDetails(item, "weightbuff");
		}
		if (10 + getItemDetails(item, "healthbuff") > maxHealth) {
			maxHealth = 10 + getItemDetails(item, "healthbuff");
		}
		if (10 + getItemDetails(item, "staminabuff") > maxStamina) {
			maxStamina = 10 + getItemDetails(item, "staminabuff");
		}
		if (getItemDetails(item, "walkease") > walkEase) {
			walkEase = getItemDetails(item, "walkease");
		}
		if (getItemDetails(item, "minebuff") > mineBuff) {
			mineBuff = getItemDetails(item, "minebuff");
		}
		if (getItemDetails(item, "mineease") > mineEase) {
			mineEase = getItemDetails(item, "mineease");
		}
		if (getItemDetails(item, "axebuff") > axeBuff) {
			axeBuff = getItemDetails(item, "axebuff");
		}
		if (getItemDetails(item, "axeease") > axeEase) {
			axeEase = getItemDetails(item, "axeease");
		}
	}
}

void timeStep(int stepMinutes) {
	timeMinutes += stepMinutes;
	timeHours += (timeMinutes / 60);
	timeMinutes = (timeMinutes % 60);
	if (timeHours > 23) {
		timeHours -= 24;
		timeDay++;
	}
	if (timeDay > 29) {
		timeDay -= 30;
		timeSeason++;
	}
	if (timeSeason > 3) {
		timeSeason -= 4;
	}
}

string underscoreToSpace(string input) {
	string output;
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '_') {
			output.push_back(' ');
		}
		else {
			output.push_back(input[i]);
		}
	}
	return output;
}

int drawMap() {
	system("cls");
	int useWidth = MAP_WIDTH;
	int useHeight = MAP_HEIGHT;
	vector<string> useMap = map;
	vector<string> invOutput;
	vector<string> menuOutput;
	invOutput.push_back("");
	if (inBase != -1) {
		useWidth = BASE_WIDTH;
		useHeight = BASE_HEIGHT;
		useMap = bases[inBase];
	}
	int y = frameLoc / useWidth;
	int x = frameLoc % useWidth;
	cout << "\n";
	onBlock = useMap[((y + (FRAME_HEIGHT / 2)) * useWidth) + x + (FRAME_WIDTH / 2)];
	for (int i = 0; i < containers.size(); i++) {
		string baseNumber;
		string baseLocation;
		bool passedColon = false;
		for (int ii = 0; ii < containers[i][0].size(); ii++) {
			if (passedColon == false && containers[i][0][ii] != ':') {
				baseNumber += containers[i][0][ii];
			}
			else if (containers[i][0][ii] == ':') {
				passedColon = true;
			}
			else {
				baseLocation += containers[i][0][ii];
			}
		}
		if (baseNumber == to_string(inBase) && baseLocation == to_string(frameLoc + 10 * BASE_WIDTH + 20)) {
			selectedContainer = i;
		}
	}
	int storageWeight = 0;
	if (onBlock == "c" && inMenu != 0) {
		menuOutput.push_back("SMALL CRAFTING TABLE");
		menuOutput.push_back("");
		if (subCommand == "") {
			menuOutput.push_back("craft: craft an item");
			menuOutput.push_back("upgrade: upgrade the crafting table");
		}
		else if (subCommand == "craft") {
			menuOutput.push_back("Use underscores for spaces in item");
			menuOutput.push_back("names.");
			menuOutput.push_back("");
			menuOutput.push_back("small hide backpack, small storage, ");
			menuOutput.push_back("stone pickaxe, stone axe, ");
			menuOutput.push_back("stone sword, leather armor, ");
			menuOutput.push_back("leather boots");
		}
		menuOutput.push_back("");
		if (subCommand != "") {
			menuOutput.push_back("back: back to main menu");
		}
		menuOutput.push_back("exit: close the menu");
	}
	else if (onBlock == "f" && inMenu != 0) {
		menuOutput.push_back("SMALL FURNACE");
		menuOutput.push_back("");
		if (subCommand == "") {
			menuOutput.push_back("cook: cook meat or refine metals");
			menuOutput.push_back("restock: add fuel to the furnace");
			menuOutput.push_back("upgrade: upgrade the furnace");
		}
		else if (subCommand == "furnaceRestock") {
			menuOutput.push_back("contains: " + to_string(count(containers[selectedContainer].begin(),containers[selectedContainer].end(),"coal")) + "x coal");
			menuOutput.push_back("");
			menuOutput.push_back("coal");
		}
		else if (subCommand == "furnaceCook") {
			menuOutput.push_back("Use underscores for spaces in item");
			menuOutput.push_back("names.");
			menuOutput.push_back("contains: " + to_string(count(containers[selectedContainer].begin(), containers[selectedContainer].end(), "coal")) + "x coal");
		}
		menuOutput.push_back("");
		if (subCommand != "") {
			menuOutput.push_back("back: back to main menu");
		}
		menuOutput.push_back("exit: close the menu");
	}
	else if (onBlock == "s" && inMenu != 0) {
		menuOutput.push_back("SMALL STORAGE");
		menuOutput.push_back("");
		vector<string> containerInventoryOutput;
		vector<string> recordedItems;
		containerInventoryOutput.push_back("");
		int invLine = 0;
		for (int i = 1; i < containers[selectedContainer].size(); i++) {
			if (count(recordedItems.begin(), recordedItems.end(), containers[selectedContainer][i]) == 0) {
				if (containerInventoryOutput[0] != "") {
					containerInventoryOutput[invLine] += ", ";
				}
				recordedItems.push_back(containers[selectedContainer][i]);
				int itemCount = count(containers[selectedContainer].begin(), containers[selectedContainer].end(), containers[selectedContainer][i]);
				storageWeight += getItemDetails(containers[selectedContainer][i], "weight") * itemCount;
				if (size(containerInventoryOutput[invLine] + to_string(itemCount) + "x " + containers[selectedContainer][i]) > 33) {
					containerInventoryOutput.push_back(to_string(itemCount) + "x " + containers[selectedContainer][i]);
					invLine++;
				}
				else {
					containerInventoryOutput[invLine] += to_string(itemCount) + "x " + containers[selectedContainer][i];
				}
			}
		}
		if (subCommand == "") {
			menuOutput.push_back("add: add items to storage");
			menuOutput.push_back("remove: remove items from storage");
			menuOutput.push_back("upgrade: upgrade the storage");
		}
		else if (subCommand == "storageAdd") {
			menuOutput.push_back("Use underscores for spaces in item");
			menuOutput.push_back("names.");
			menuOutput.push_back("weight: " + to_string(storageWeight) + " / 100");
			menuOutput.push_back("");
			for (int i = 0; i < containerInventoryOutput.size(); i++) {
				menuOutput.push_back(containerInventoryOutput[i]);
			}
		}
		else if (subCommand == "storageRemove") {
			menuOutput.push_back("Use underscores for spaces in item");
			menuOutput.push_back("names.");
			menuOutput.push_back("weight: " + to_string(storageWeight) + " / 100");
			menuOutput.push_back("");
			for (int i = 0; i < containerInventoryOutput.size(); i++) {
				menuOutput.push_back(containerInventoryOutput[i]);
			}
		}
		menuOutput.push_back("");
		if (subCommand != "") {
			menuOutput.push_back("back: back to main menu");
		}
		menuOutput.push_back("exit: close the menu");
	}
	for (int fy = 0; fy < FRAME_HEIGHT; fy++) {
		for (int fx = 0; fx < FRAME_WIDTH; fx++) {
			if (fx == 0) {
				cout << "  ";
			}
			if (inMenu == 0) {
				if (fy == FRAME_HEIGHT / 2 && fx == FRAME_WIDTH / 2) {
					if (health > 0) {
						cout << "P";
					}
					else {
						cout << "X";
					}
				}
				else {
					if (((y + fy) * useWidth) + x + fx >= 0 && ((y + fy) * useWidth) + x + fx < useWidth * useHeight) {
						cout << useMap[((y + fy) * useWidth) + x + fx];
					}
					else {
						cout << " ";
					}
				}
			}
			else {
				if ((fy == 0 && (fx == 0 || fx == FRAME_WIDTH - 1)) || (fy == FRAME_HEIGHT - 1 && (fx == 0 || fx == FRAME_WIDTH - 1))) {
					cout << "+";
				}
				else if (fy == 0 || fy == FRAME_HEIGHT - 1) {
					cout << "-";
				}
				else if (fx == FRAME_WIDTH - 1 || fx == 0) {
					cout << "|";
				}
				else {
					if (fy >= 2 && menuOutput.size() > fy - 2) {
						if (fx >= 3 && menuOutput[fy - 2].length() > fx - 3) {
							cout << menuOutput[fy - 2][fx - 3];
						}
						else {
							cout << " ";
						}
					}
					else {
						cout << " ";
					}
				}
			}
			if (fx == FRAME_WIDTH - 1 && fy == 0) {
				string showSeason = "";
				if (timeSeason == 0) showSeason = "Spring";
				if (timeSeason == 1) showSeason = "Summer";
				if (timeSeason == 2) showSeason = "Fall";
				if (timeSeason == 3) showSeason = "Winter";
				cout << "  TIME:    " << showSeason << " " << timeDay << ", ";
				if (timeHours < 10) cout << "0";
				cout << timeHours << ":";
				if (timeMinutes < 10) cout << "0";
				cout << timeMinutes << ":00 ";
			}
			if (fx == FRAME_WIDTH - 1 && fy == 2) {
				cout << "  HEALTH:  [";
				for (int i = 0; i < maxHealth; i++) {
					if (i < health) {
						cout << "X";
					}
					else {
						cout << "_";
					}
				}
				cout << "]";
			}
			if (fx == FRAME_WIDTH - 1 && fy == 3) {
				cout << "  STAMINA: [";
				for (int i = 0; i < maxStamina; i++) {
					if (i < stamina) {
						cout << "X";
					}
					else {
						cout << "_";
					}
				}
				cout << "]";
			}
			if (fx == FRAME_WIDTH - 1 && fy == 4) {
				cout << "  HUNGER:  [";
				for (int i = 0; i < maxHunger; i++) {
					if (i < hunger) {
						cout << "X";
					}
					else {
						cout << "_";
					}
				}
				cout << "]";
			}
			if (fx == FRAME_WIDTH - 1 && fy == 5) {
				cout << "  WEIGHT:  " << weight << " / " << weightCapacity;
			}
			if (fx == FRAME_WIDTH - 1 && fy == 7) {
				cout << "  INVENTORY: ";
				if (inventory.size() == 0) {
					cout << "empty";
				}
				else {
					vector<string> alreadyRecorded;
					int onLine = 0;
					for (int i = 0; i < inventory.size(); i++) {
						if (count(alreadyRecorded.begin(), alreadyRecorded.end(), inventory[i]) == 0) {
							string toOutput;
							if (invOutput[0] != "") {
								invOutput[onLine] += ", ";
							}
							int countItem = count(inventory.begin(), inventory.end(), inventory[i]);
							toOutput = to_string(countItem) + "x " + inventory[i];
							if (invOutput[onLine].size() + toOutput.size() > 59) {
								invOutput.push_back(toOutput);
								onLine++;
							}
							else {
								invOutput[onLine] += toOutput;
							}
							alreadyRecorded.push_back(inventory[i]);
						}
					}
					cout << invOutput[0];
				}
			}
			if (fx == FRAME_WIDTH - 1 && fy > 7) {
				if (invOutput.size() > fy - 7) {
					cout << "             " << invOutput[fy - 7];
				}
			}
		}
		cout << "\n";
	}
	cout << "\n  " << outputText << "\n\n  > ";
	string s;
	cin >> s;
	if (subCommand == "") {
		if (s == "d" && x < useWidth - FRAME_WIDTH && health > 0 && stamina > 0) {
			frameLoc++;
			outputText = "You moved west one chunk.";
			int wER = rand() % walkEase; // walk ease random variable
			if (inBase == -1) {
				if (wER == 0) {
					dropHunger(1);
				}
				timeStep(10);
			}
		}
		else if (s == "a" && x > 0 && health > 0 && stamina > 0) {
			frameLoc--;
			outputText = "You moved east one chunk.";
			int wER = rand() % walkEase; // walk ease random variable
			if (inBase == -1) {
				if (wER == 0) {
					dropHunger(1);
				}
				timeStep(10);
			}
		}
		else if (s == "w" && y > 0 && health > 0 && stamina > 0) {
			frameLoc -= useWidth;
			outputText = "You moved north one chunk.";
			int wER = rand() % walkEase; // walk ease random variable
			if (inBase == -1) {
				if (wER == 0) {
					dropHunger(1);
				}
				timeStep(10);
			}
		}
		else if (s == "s" && y < useHeight - FRAME_HEIGHT && health > 0 && stamina > 0) {
			frameLoc += useWidth;
			outputText = "You moved south one chunk.";
			int wER = rand() % walkEase; // walk ease random variable
			if (inBase == -1) {
				if (wER == 0) {
					dropHunger(1);
				}
				timeStep(10);
			}
		}
		else if ((s == "d" && x >= useWidth - FRAME_WIDTH) || (s == "a" && x <= 0) || (s == "w" && y <= 0) || (s == "s" && y >= useHeight - FRAME_HEIGHT)) {
			moveOutOfBounds();
		}
		else if (health < 1) {}
		else if (s == "hurt") {
			health--;
			outputText = "You were hurt and lost 1 health.";
		}
		else if (s == "hunt" && stamina > 1 && inBase == -1) {
			if (onBlock == "," || onBlock == "H" || onBlock == "M") {
				int bB = 0; // biome boost for hide;
				if (onBlock == "M") {
					bB = 3;
				}
				int mGR = rand() % 4; // meat gained random variable (no tools)
				int hGR = rand() % 2 + bB; // hide gained random variable (no tools)
				outputText = "You went hunting. (+" + to_string(mGR) + " raw meat, +" + to_string(hGR) + " raw hide)";
				int droppedMeat = 0;
				for (int i = 0; i < mGR; i++) {
					if (weight + getItemDetails("raw meat", "weight") <= weightCapacity) {
						addItemToInventory("raw meat");
					}
					else {
						droppedMeat++;
					}
				}
				for (int i = 0; i < hGR; i++) {
					if (weight + getItemDetails("raw hide", "weight") <= weightCapacity) {
						addItemToInventory("raw hide");
					}
					else {
						droppedMeat++;
					}
				}
				if (droppedMeat > 0) {
					outputText = "You went hunting and gained " + to_string(mGR + hGR) + " items, but had to drop " + to_string(droppedMeat) + " because you couldn't carry it.";
				}
				dropHunger(2);
				timeStep(30);
			}
			else {
				outputText = "You cannot hunt here.";
			}
		}
		else if (s == "hunt" && stamina < 2 && inBase == -1) {
			outputText = "You are too tired to hunt.";
		}
		else if (s == "fish" && stamina > 1 && inBase == -1) {
			if (onBlock == "~") {
				int mGR = rand() % 3; // meat gained random variable (no tools)
				outputText = "You went fishing. (+" + to_string(mGR) + " raw fish)";
				int droppedMeat = 0;
				for (int i = 0; i < mGR; i++) {
					if (weight + getItemDetails("raw fish", "weight") <= weightCapacity) {
						addItemToInventory("raw fish");
					}
					else {
						droppedMeat++;
					}
				}
				if (droppedMeat > 0) {
					outputText = "You went fishing and gained " + to_string(mGR) + " raw fish, but had to drop " + to_string(droppedMeat) + " because you couldn't carry it.";
				}
				dropHunger(2);
				timeStep(60);
			}
			else {
				outputText = "You cannot fish here.";
			}
		}
		else if (s == "fish" && stamina < 2 && inBase == -1) {
			outputText = "You are too tired to fish.";
		}
		else if (s == "eat") {
			subCommand = "eat";
			outputText = "Which item? Type underscore for spaces (e.g.: raw_meat)";
		}
		else if (s == "mine" && inBase == -1) {
			if (onBlock == "#" || onBlock == "C" || onBlock == "I" || onBlock == "S" || onBlock == "T") {
				string itemType = "";
				int actionEase = 1;
				int actionBuff = 0;
				if (onBlock == "#") {
					itemType = "stone";
					actionEase = mineEase;
					actionBuff = mineBuff;
				}
				else if (onBlock == "C") {
					itemType = "coal";
					actionEase = mineEase;
					actionBuff = mineBuff;
				}
				else if (onBlock == "I") {
					itemType = "iron";
					actionEase = mineEase;
					actionBuff = mineBuff;
				}
				else if (onBlock == "S") {
					itemType = "steel";
					actionEase = mineEase;
					actionBuff = mineBuff;
				}
				else if (onBlock == "T") {
					itemType = "wood";
					actionEase = axeEase;
					actionBuff = axeBuff;
				}
				if (stamina > getItemDetails(itemType, "hardness") - 1) {
					dropHunger(getItemDetails(itemType, "hardness") / actionEase);
					timeStep(getItemDetails(itemType, "hardness")*10);
					int mR = rand() % getItemDetails(itemType, "drop") + actionBuff; // mine random variable
					outputText = "You went mining. (+" + to_string(mR) + " " + itemType + ")";
					int droppedItems = 0;
					for (int i = 0; i < mR; i++) {
						if (weight + getItemDetails(itemType, "weight") <= weightCapacity) {
							addItemToInventory(itemType);
						}
						else {
							droppedItems++;
						}
					}
					if (droppedItems > 0) {
						outputText = "You went mining and gained " + to_string(mR) + " " + itemType + ", but had to drop " + to_string(droppedItems) + " because you couldn't carry it.";
					}
				}
				else {
					outputText = "You are too tired to mine.";
				}
			}
			else {
				outputText = "You cannot mine here.";
			}
		}
		else if (s == "build" && inBase == -1) {
			if (onBlock != "~") {
				if (stamina > 9 && hunger > 4) {
					int woodCount = count(inventory.begin(), inventory.end(), "wood");
					int stoneCount = count(inventory.begin(), inventory.end(), "stone");
					if (woodCount > 19 && stoneCount > 4) {
						dropHunger(15);
						timeStep(180);
						map[frameLoc + 10 * MAP_WIDTH + 20] = "B";
						vector<string> newBase;
						for (int i = 0; i < BASE_WIDTH * BASE_HEIGHT; i++) {
							int y = i / BASE_WIDTH;
							int x = i % BASE_WIDTH;
							if (y == 11 && x == 21) {
								newBase.push_back("c");
							}
							else if (y == 19 && x == 21) {
								newBase.push_back("s");
								vector<string> storageVector;
								storageVector.push_back(to_string(bases.size()) + ":" + to_string(i));
								containers.push_back(storageVector);
							}
							else if (y == 11 && x == 39) {
								newBase.push_back("f");
								vector<string> storageVector;
								storageVector.push_back(to_string(bases.size()) + ":" + to_string(i));
								containers.push_back(storageVector);
							}
							else if (y > 9 && y < 21 && x > 19 && x < 41) {
								newBase.push_back("#");
							}
							else if (((y == 9 || y == 21) && x > 18 && x < 42) || ((x == 19 || x == 41) && y > 8 && y < 22)) {
								newBase.push_back("W");
							}
							else {
								newBase.push_back(onBlock);
							}
						}
						newBase.push_back(to_string(frameLoc + 10 * MAP_WIDTH + 20));
						bases.push_back(newBase);
						outputText = "You built successfully. Come back here to store, craft, and cook items. Type enter to go inside.";
						removeItemFromInventory("wood", 20);
						removeItemFromInventory("stone", 5);
					}
					else {
						outputText = "You do not have enough resources to build. (Needed: 20 wood, 5 stone)";
					}
				}
				else {
					outputText = "You are too tired to build";
				}
			}
			else {
				outputText = "You cannot build here.";
			}
		}
		else if (s == "drop") {
			subCommand = "drop";
			outputText = "Which item? Type underscore for spaces (e.g.: raw_meat)";
		}
		else if (s == "enter" && inBase == -1) {
			if (onBlock == "B") {
				for (int i = 0; i < bases.size(); i++) {
					if (bases[i][bases[i].size()-1] == to_string(frameLoc + 10 * MAP_WIDTH + 20)) {
						inBase = i;
						worldFrameLoc = frameLoc;
						frameLoc = ((BASE_WIDTH / 2) - (FRAME_WIDTH / 2)) + (((BASE_HEIGHT / 2) * BASE_WIDTH) - ((FRAME_HEIGHT / 2) * BASE_WIDTH));
						outputText = "You entered base #" + to_string(inBase + 1);
					}
				}
			}
			else {
				outputText = "There is nothing to enter.";
			}
		}
		else if (s == "pickup" && inBase != -1) {
			if (onBlock == "c" || onBlock == "s" || onBlock == "f") {
				string itemToPickup;
				if (onBlock == "c") {
					itemToPickup = "small crafting table";
				}
				else if (onBlock == "f") {
					itemToPickup = "small furnace";
				}
				else if (onBlock == "s") {
					itemToPickup = "small storage";
				}
				if (weight + getItemDetails(itemToPickup, "weight") <= weightCapacity) {
					addItemToInventory(itemToPickup);
					outputText = "You picked up 1x " + itemToPickup + ".";
					bases[inBase][frameLoc + 10 * BASE_WIDTH + 20] = "#";
					if (selectedContainer != -1) {
						if (onBlock == "s") {
							for (int i = 1; i < containers[selectedContainer].size(); i++) {
								if (weight + getItemDetails(containers[selectedContainer][i], "weight") <= weightCapacity) {
									addItemToInventory(containers[selectedContainer][i]);
								}
							}
						}
						containers.erase(containers.begin() + selectedContainer);
					}
				}
			}
			else {
				outputText = "You cannot pickup this item.";
			}
		}
		else if (s == "place" && inBase != -1) {
			if (onBlock == "#") {
				subCommand = "place";
				outputText = "Which item? Type underscore for spaces (e.g.: small_crafting_table)";
			}
			else {
				outputText = "You cannot place an item here.";
			}
		}
		else if (s == "use" && inBase != -1) {
			if (onBlock == "c" || onBlock == "s" || onBlock == "f") {
				inMenu = 1;
				outputText = "";
			}
			else {
				outputText = "You cannot use this item.";
			}
		}
		else if (s == "craft" && inMenu == 1 && onBlock == "c") {
			subCommand = "craft";
			outputText = "";
		}
		else if (s == "restock" && inMenu == 1 && onBlock == "f") {
			subCommand = "furnaceRestock";
			outputText = "";
		}
		else if (s == "add") {
			subCommand = "storageAdd";
			outputText = "";
		}
		else if (s == "remove") {
			subCommand = "storageRemove";
			outputText = "";
		}
		else if (s == "cook") {
			subCommand = "furnaceCook";
			outputText = "";
		}
		else if (s == "help" && inBase == -1) {
			outputText = "w, a, s, d, help, fish, hunt, eat, drop, mine, build, enter";
		}
		else if (s == "help" && inBase != -1) {
			outputText = "w, a, s, d, help, eat, drop, exit, use, pickup, place";
}
		else if (s == "rest" && health > 0) {
			if (stamina < maxStamina) {
				stamina++;
				outputText = "You rested and restored 1 stamina.";
				timeStep(60);
			}
			else {
				outputText = "You are not tired.";
			}
		}
		else if (stamina < 1) {
			outputText = "You are too tired to move.";
		}
		else if (s == "exit" && inBase != -1) {
			if (inMenu == 0) {
				inBase = -1;
				frameLoc = worldFrameLoc;
				outputText = "You exited your base.";
			}
			else {
				inMenu = 0;
				outputText = "You stopped using an item.";
			}
		}
		else {
			outputText = "Command unavailable. Type help for a list of commands.";
		}
	}
	else if (subCommand == "eat") {
		string itemType = underscoreToSpace(s);
		if (getItemDetails(itemType, "edible") == 1) {
			if (count(inventory.begin(), inventory.end(), itemType) > 0) {
				removeItemFromInventory(itemType);
				hunger = hunger + getItemDetails(itemType, "nutrition");
				if (hunger > maxHunger) {
					hunger = maxHunger;
				}
				health = health + getItemDetails(itemType, "healing");
				if (health > maxHealth) {
					health = maxHealth;
				}
				outputText = "You ate 1x " + itemType + " and regained " + to_string(getItemDetails(itemType, "nutrition")) + " hunger and " + to_string(getItemDetails(itemType, "healing")) + " health.";
			}
			else {
				outputText = "You do not have any of those.";
			}
		}
		else {
			outputText = "You cannot eat this item.";
		}
		subCommand = "";
	}
	else if (subCommand == "drop") {
		string itemToDrop = underscoreToSpace(s);
		int itemsInInventory = count(inventory.begin(), inventory.end(), itemToDrop);
		if (itemsInInventory > 0) {
			removeItemFromInventory(itemToDrop);
			outputText = "Dropped 1x " + itemToDrop + ".";
		}
		else {
			outputText = "You don't have that item to drop.";
		}
		subCommand = "";
	}
	else if (subCommand == "place") {
		string itemToPlace = underscoreToSpace(s);
		if (s == "small_crafting_table") {
			bases[inBase][frameLoc + 10 * BASE_WIDTH + 20] = "c";
		}
		else if (s == "small_furnace") {
			bases[inBase][frameLoc + 10 * BASE_WIDTH + 20] = "f";
			vector<string> storageVector;
			storageVector.push_back(to_string(inBase) + ":" + to_string(frameLoc + 10 * BASE_WIDTH + 20));
			containers.push_back(storageVector);
		}
		else if (s == "small_storage") {
			bases[inBase][frameLoc + 10 * BASE_WIDTH + 20] = "s";
			vector<string> storageVector;
			storageVector.push_back(to_string(inBase) + ":" + to_string(frameLoc + 10 * BASE_WIDTH + 20));
			containers.push_back(storageVector);
		}
		if (getItemDetails(itemToPlace, "placeable") == 1) {
			if (count(inventory.begin(), inventory.end(), itemToPlace) > 0) {
				removeItemFromInventory(itemToPlace);
				outputText = "You placed 1x " + itemToPlace + ".";
			}
			else {
				outputText = "You do not have any of these to place.";
			}
		}
		else {
			outputText = "You cannot place that item.";
		}
		subCommand = "";
	}
	else if (subCommand == "craft") {
		if (s == "small_hide_backpack") {
			int hideInInventory = count(inventory.begin(), inventory.end(), "raw hide");
			if (hideInInventory > 4) {
				addItemToInventory("small hide backpack");
				removeItemFromInventory("raw hide", 5);
				outputText = "\"small hide backpack\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 5x raw hide)";
			}
		}
		else if (s == "small_storage") {
			int woodInInventory = count(inventory.begin(), inventory.end(), "wood");
			if (woodInInventory > 4) {
				addItemToInventory("small storage");
				removeItemFromInventory("wood", 5);
				outputText = "\"small storage\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 5x wood)";
			}
		}
		else if (s == "stone_pickaxe") {
			int woodInInventory = count(inventory.begin(), inventory.end(), "wood");
			int stoneInInventory = count(inventory.begin(), inventory.end(), "stone");
			if (woodInInventory > 2 && stoneInInventory > 4) {
				addItemToInventory("stone pickaxe");
				removeItemFromInventory("wood", 3);
				removeItemFromInventory("stone", 5);
				outputText = "\"stone pickaxe\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 3x wood, 5 stone)";
			}
		}
		else if (s == "stone_axe") {
			int woodInInventory = count(inventory.begin(), inventory.end(), "wood");
			int stoneInInventory = count(inventory.begin(), inventory.end(), "stone");
			if (woodInInventory > 4 && stoneInInventory > 2) {
				addItemToInventory("stone axe");
				removeItemFromInventory("wood", 5);
				removeItemFromInventory("stone", 3);
				outputText = "\"stone axe\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 5x wood, 3 stone)";
			}
		}
		else if (s == "stone_sword") {
			int woodInInventory = count(inventory.begin(), inventory.end(), "wood");
			int stoneInInventory = count(inventory.begin(), inventory.end(), "stone");
			if (woodInInventory > 4 && stoneInInventory > 4) {
				addItemToInventory("stone sword");
				removeItemFromInventory("wood", 5);
				removeItemFromInventory("stone", 5);
				outputText = "\"stone sword\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 5x wood, 5 stone)";
			}
		}
		else if (s == "leather_armor") {
			int hideInInventory = count(inventory.begin(), inventory.end(), "raw hide");
			if (hideInInventory > 29) {
				addItemToInventory("leather armor");
				removeItemFromInventory("raw hide", 30);
				outputText = "\"leather armor\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 30x raw hide)";
			}
		}
		else if (s == "leather_boots") {
			int hideInInventory = count(inventory.begin(), inventory.end(), "raw hide");
			if (hideInInventory > 14) {
				addItemToInventory("leather boots");
				removeItemFromInventory("raw hide", 15);
				outputText = "\"leather boots\" crafted successfully.";
			}
			else {
				outputText = "You do not have enough materials to craft this item. (required: 15x raw hide)";
			}
		}
		else {
			outputText = "You cannot craft that item.";
		}
	}
	else if (subCommand == "furnaceRestock") {
		if (s == "coal") {
			if (count(inventory.begin(), inventory.end(), "coal") > 0) {
				containers[selectedContainer].push_back("coal");
				removeItemFromInventory("coal");
			}
			else {
				outputText = "You do not have enough of those to put in the furnace.";
			}
		}
		else {
			outputText = "You cannot use that item to restock the furnace.";
		}
	}
	else if (subCommand == "storageAdd") {
		string itemToPlaceInStorage = underscoreToSpace(s);
		if (count(inventory.begin(), inventory.end(), itemToPlaceInStorage) > 0) {
			if (storageWeight + getItemDetails(itemToPlaceInStorage, "weight") <= 100) {
				containers[selectedContainer].push_back(itemToPlaceInStorage);
				removeItemFromInventory(itemToPlaceInStorage);
			}
			else {
				outputText = "There is not enough room in this storage for this item.";
			}
		}
		else {
			outputText = "You do not have enough of those to put in storage.";
		}
	}
	else if (subCommand == "storageRemove") {
		string itemToRemoveFromStorage = underscoreToSpace(s);
		if (count(containers[selectedContainer].begin(), containers[selectedContainer].end(), itemToRemoveFromStorage) > 0) {
			if (weight + getItemDetails(itemToRemoveFromStorage, "weight") <= weightCapacity) {
				bool removedFromStorage = false;
				for (int i = 0; i < containers[selectedContainer].size(); i++ && removedFromStorage == false) {
					if (containers[selectedContainer][i] == itemToRemoveFromStorage) {
						containers[selectedContainer].erase(containers[selectedContainer].begin() + i);
						removedFromStorage = true;
					}
				}
				addItemToInventory(itemToRemoveFromStorage);
			}
			else {
				outputText = "You can't carry that item.";
			}
		}
		else {
			outputText = "You can't find that item.";
		}
	}
	else if (subCommand == "furnaceCook") {
		string itemToCook = underscoreToSpace(s);
		if (count(inventory.begin(), inventory.end(), itemToCook) > 0) {
			if (getItemDetails(itemToCook, "cookable") == 1) {
				if (containers[selectedContainer].size() > 1) {
					string goesTo;
					if (itemToCook == "raw meat") {
						goesTo = "cooked meat";
					}
					else if (itemToCook == "raw fish") {
						goesTo = "cooked fish";
					}
					removeItemFromInventory(itemToCook);
					addItemToInventory(goesTo);
					string useFuel = containers[selectedContainer][1];
					int useChance = 1;
					if (useFuel == "coal") {
						useChance = 4;
					}
					int uR = rand() % useChance; // use random variable
					if (uR == 0) {
						containers[selectedContainer].erase(containers[selectedContainer].begin() + 1);
					}
				}
				else {
					outputText = "You don't have any fuel to cook this item.";
				}
			}
			else {
				outputText = "You can't cook that item.";
			}
		}
		else {
			outputText = "You do not have enough of those to cook.";
		}
	}
	if (s == "back" && inMenu == 1 && subCommand != "") {
		subCommand = "";
		outputText = "";
	}
	if (s == "exit" && inMenu == 1 && subCommand != "") {
		subCommand = "";
		outputText = "You stopped using an item.";
		inMenu = 0;
	}
	if (health < 1) {
		outputText = "You died.";
	}
	selectedContainer = -1; // cleaning up some stuff...
	if (s != "stop" && s != "quit") {
		return 1;
		drawMap();
	}
	return 0;
}

void spreadChunk(long int spreader, string spreadee) {
	chunkSpreadChance -= CHUNK_DIE;
	int sCR = rand() % 1000; // the spread chance random variable;
	if (sCR < chunkSpreadChance) {
		if (spreader - 1 >= 0 && spreader - 1 < MAP_WIDTH * MAP_HEIGHT && map[spreader - 1] == "#") {
			map[spreader - 1] = spreadee;
			spreadChunk(spreader - 1, spreadee);
		}
		if (spreader + 1 >= 0 && spreader + 1 < MAP_WIDTH * MAP_HEIGHT && map[spreader + 1] == "#") {
			map[spreader + 1] = spreadee;
			spreadChunk(spreader + 1, spreadee);
		}
		if (spreader - MAP_WIDTH >= 0 && spreader - MAP_WIDTH < MAP_WIDTH * MAP_HEIGHT && map[spreader - MAP_WIDTH] == "#") {
			map[spreader - MAP_WIDTH] = spreadee;
			spreadChunk(spreader - MAP_WIDTH, spreadee);
		}
		if (spreader + MAP_WIDTH >= 0 && spreader + MAP_WIDTH < MAP_WIDTH * MAP_HEIGHT && map[spreader + MAP_WIDTH] == "#") {
			map[spreader + MAP_WIDTH] = spreadee;
			spreadChunk(spreader + MAP_WIDTH, spreadee);
		}
	}
}

void genMap() {
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
		map.push_back("#");
	}
	for (int i = 0; i < CHUNK_COUNT; i++) {
		int tSR = rand() % 7; // to spread random variable
		string toSpread = "!";
		chunkSpreadChance = CHUNK_SPREAD;
		if (tSR == 0) {
			toSpread = ",";
		}
		else if (tSR == 1) {
			toSpread = "M";
		}
		else if (tSR == 2) {
			toSpread = "~";
		}
		else if (tSR == 3) {
			toSpread = "H";
		}
		else if (tSR == 4) {
			toSpread = "C";
			chunkSpreadChance = CHUNK_SPREAD / 1.75;
		}
		else if (tSR == 5) {
			toSpread = "I";
			chunkSpreadChance = CHUNK_SPREAD / 2;
		}
		else if (tSR == 6) {
			toSpread = "S";
			chunkSpreadChance = CHUNK_SPREAD / 2.5;
		}
		int chunkStartLoc = rand() % (MAP_WIDTH * MAP_HEIGHT);
		if (map[chunkStartLoc] == "#") {
			map[chunkStartLoc] = toSpread;
			spreadChunk(chunkStartLoc, toSpread);
		}
	}
	for (int i = 0; i < TREE_COUNT; i++) {
		int treeLoc = rand() % (MAP_WIDTH * MAP_HEIGHT);
		if (map[treeLoc] == "," || map[treeLoc] == "H") {
			map[treeLoc] = "T";
		}
	}
}

int main()
{
	srand(MAP_SEED);
	cout << "Generating Map (SEED: " << MAP_SEED << ")...\n";
	genMap();
	cout << "Drawing Map...\n";
	bool running = true;
	while (running) {
		int rC = drawMap();
		if (rC == 0) {
			running = false;
		}
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
