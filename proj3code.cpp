//----------------------------------------------------------------------------
//                              Golf Club App
//----------------------------------------------------------------------------
// CS 215 - Spring 2017 - Project 3
// Name:     Paul Barrett
// Section:  009
// Date:     April 7, 2017
// An application to manage data for a Golf Club, including:
// - individual scores on a single hole of golf
// - total scores for a game
// - a list of games for one player
// - a list of players for the whole club
//----------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

const int MAX_PLAYERS = 10;		// max number of players in a club
const int MAX_HOLES = 18;		// max number of holes in a game
const int MAX_GAMES = 10;		// max number of games for a player
const int NOT_FOUND = -1;		// return code for search
const int EXIT_OPTION = 7;		// main menu option for EXIT

struct aGame {					// WRITE a structure for aGame. Members:
	string course;				// course      - string         - name of golf course
	string date;				// date        - string         - date game was played
	int totalScore;		        // totalScore  - integer        - total score for the game
	int hole[MAX_HOLES];		// hole        - array of ints  - list of scores for each hole
	int numHoles;				// numHoles    - integers       - number of holes played
};								

struct aPlayer {
	string name;				// name of player
	float  avgScore;			// player's average score on all games
	int    numGames;			// number of games this player has played
	aGame  game[MAX_GAMES];		// list of games played by this player
};

struct aClub {
	string  name;				// name of the club
	int     numPlayers;			// number of players in the club
	int		holeAvg;			// used to calculate total score for players
	aPlayer player[MAX_PLAYERS];// list of players in this club
};

//----------------------------------------------------------------
//                         printLogo
//----------------------------------------------------------------
// design a new logo if you wish (include your Name and Section)
void printLogo() {
	system("cls");
	cout << "+------------------------------------------------+\n";
	cout << "|                GOLB CLUB EXPRESS PRO           |\n";
	cout << "|               by Paul Barrett - 009            |\n";
	cout << "+------------------------------------------------+\n";
} // printLogo()


  //-----------------------------------------------------------------------------
  //                                 initGame
  //-----------------------------------------------------------------------------
void initGame(aGame & g) {
	g.course = "";
	g.date = "";
	g.totalScore = 0;
	for (int i = 0; i < MAX_HOLES; i++)
		g.hole[i] = 0;
	g.numHoles = 0;
} // initGame()

  //-----------------------------------------------------------------------------
  //                                 initPlayer
  //-----------------------------------------------------------------------------
void initPlayer(aPlayer & p) {
	p.name = "";
	p.avgScore = 0.0;
	p.numGames = 0;
	for (int i = 0; i < MAX_GAMES; i++)
		initGame(p.game[i]);
} // initPlayer()

  //-----------------------------------------------------------------------------
  //                                 initClub
  //-----------------------------------------------------------------------------
void initClub(aClub & c) {
	c.name = "";
	c.numPlayers = 0;
	c.holeAvg = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
		initPlayer(c.player[i]);
} // initClub()

  //-----------------------------------------------------------------------------
  //                               totalScore
  //-----------------------------------------------------------------------------
  // Given: the index of the player and game
  // Calculates: total score for that game
  //-----------------------------------------------------------------------------
int totalScore(aClub & c, int index, int gindex) {
	int sum = 0;
	int ts = 0;
	for (int i = 0; i < c.player[index].game[gindex].numHoles; i++) {
		sum += c.player[index].game[gindex].hole[i];
	}
	ts = sum + (18 - c.player[index].game[gindex].numHoles)*c.holeAvg;
	return ts;
}

//-----------------------------------------------------------------------------
//                               calcTotals
//-----------------------------------------------------------------------------
// Uses totalScore function to calculate totals for every game for every player
// Calculates all score totals
//-----------------------------------------------------------------------------
void calcTotals(aClub & c) {
	for (int i = 0; i < c.numPlayers; i++) {
		for (int j = 0; j < c.player[i].numGames; j++) {
			for (int k = 0; k < c.player[i].game[j].numHoles; k++) {
				c.player[i].game[j].totalScore = totalScore(c, i, j);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//                               calcAvgs
//-----------------------------------------------------------------------------
// Given: calculated totals, calculates average for each player
// Calculates average score for each player
//-----------------------------------------------------------------------------
void calcAvgs(aClub & c) {
	float sum = 0;
	float count = 0;
	for (int i = 0; i < c.numPlayers; i++) {
		for (int k = 0; k < c.player[i].numGames; k++) {
			if (c.player[i].numGames == 1) {
				c.player[i].avgScore = c.player[i].game[k].totalScore;
			}
			else {
				sum += c.player[i].game[k].totalScore;
				count++;
				c.player[i].avgScore = sum / count;
			}
		}
	}
}

  //-----------------------------------------------------------------------------
  //                                 startUp
  //-----------------------------------------------------------------------------
  // Modifies: a Club structure
  // Initializes the the club to "empty", then reads data from a file or
  // starts a new club.
  //-----------------------------------------------------------------------------
void startUp(aClub & c) {
	string filename = "";
	initClub(c);
	cout << "Enter input file name, or 'new' to start a new club: ";
	cin >> filename;
	cin.ignore();
	if (filename == "new") {
		cout << "Enter the Club Name: ";
		getline(cin, c.name);
		cout << "Enter Club Hole Average: ";
		cin >> c.holeAvg;
		}
		else {
			ifstream f;
			f.open(filename);
			if (f.fail()) {
				cout << "Unable to open " << filename;
				cout << "\nStarting a new club instead.";
				cout << "\nEnter the Club Name: ";
				getline(cin, c.name);
				cout << "Enter Club Hole Average: ";
				cin >> c.holeAvg;
			}
			f >> c.numPlayers >> c.holeAvg;										//num players and club hole avg
			f >> ws;															//clear whitespace before club name
			getline(f, c.name);													//club name
			for (int i = 0; i < c.numPlayers; i++) {							//player info loop
				f >> c.player[i].numGames;										//player num games
				f >> ws;
				getline(f, c.player[i].name);									//player name
				for (int j = 0; j < c.player[i].numGames; j++) {				//game info loop
					f >> c.player[i].game[j].numHoles;							//player game numholes
					f >> c.player[i].game[j].date;								//player game date
					f >> ws;
					getline(f, c.player[i].game[j].course);						//player game course
					for (int k = 0; k < c.player[i].game[j].numHoles; k++) {	//game hole array loop
						f >> c.player[i].game[j].hole[k];						//player game hole
					}
				}
			}
			f.close();
		}
} // startUp()


  //-----------------------------------------------------------------------------
  //                                 printClub
  //-----------------------------------------------------------------------------
void printClub(aClub & c) {
	printLogo();
	calcAvgs(c);
	calcTotals(c);
	cout << "--------------------------------------------------------------------" << endl;
	cout << "	          " << c.name << "                                       " << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "         #Players: " << c.numPlayers << "  Club Hole Avg: " << c.holeAvg << endl;
	for (int i = 0; i < c.numPlayers; i++) {
		if (i > 0)
			cout << endl;
		cout << c.player[i].name << " Avg: " << c.player[i].avgScore << " #Games: " << c.player[i].numGames << endl;
		for (int j = 0; j < c.player[i].numGames; j++) {
			cout << "     " << c.player[i].game[j].date << " Score: " << c.player[i].game[j].totalScore << " #Holes: " << c.player[i].game[j].numHoles << " Course: " << c.player[i].game[j].course << endl;
			for (int k = 0; k < c.player[i].game[j].numHoles; k++) {
				if (k == 0)
					cout << "     [";
				cout << c.player[i].game[j].hole[k];
				if (k < c.player[i].game[j].numHoles - 1) {
					cout << ", ";
				}
				else {
					cout << "]" << endl;
				}
			}
		}
	}
} // printClub()

  //-----------------------------------------------------------------------------
  //                               addPlayer
  //-----------------------------------------------------------------------------
  // Modifies: a club structure
  // If the list of players is not full, initializes the next available element
  // in the club's player list and asks the user for the new player's name.
  //-----------------------------------------------------------------------------
void addPlayer(aClub & c) {
	printLogo();
	if (c.numPlayers == MAX_PLAYERS) {
		cout << "ERROR: Club is full.\n";
	}
	else {
		cout << "Enter player's name:";
		cin.ignore();
		getline(cin, c.player[c.numPlayers].name);
		c.numPlayers++;
		cout << "Player added to club!\n";
	}
} // addPlayer()

  //-----------------------------------------------------------------------------
  //                               dropPlayer
  //-----------------------------------------------------------------------------
  // Modifies: a club structure
  // Allows the user to select a player for deletion from the club's Player list
  // and deletes it.
  //-----------------------------------------------------------------------------
void dropPlayer(aClub & c) {
	string dropSel;
	int playerFound = 0;
	int index = 0;
	printLogo();
	for (int i = 0; i < c.numPlayers; i++) {
		cout << c.player[i].name << endl;
	}
	cout << "Enter player's name:";
	cin.ignore();
	getline(cin, dropSel);
	for (int j = 0; j < c.numPlayers; j++) {
		if (dropSel == c.player[j].name) {
			index = j;
			playerFound = 1;
		}
	}
	if (playerFound == 1) {
			for (int k = index; k < c.numPlayers; k++) {
				c.player[k] = c.player[k + 1];
			}
		c.numPlayers--;
		initPlayer(c.player[c.numPlayers]);
		cout << "Player dropped.";
		playerFound = 0;
		
		}
		else {
			cout << "No player named " << dropSel << " in the club.";
		}
} // dropPlayer()

  //-----------------------------------------------------------------------------
  //                               addGameHelp
  //-----------------------------------------------------------------------------
  // Helper function than handles else case in addGame
  // Adds game data for a new game to a player in the club
  //-----------------------------------------------------------------------------
void addGameHelp(aClub & c, int index) {
	c.player[index].numGames++;
	cout << "Enter course name: ";
	getline(cin, c.player[index].game[c.player[index].numGames - 1].course);
	cout << "Enter date played: ";
	cin >> c.player[index].game[c.player[index].numGames - 1].date;
	cout << "Enter number of holes:";
	cin >> c.player[index].game[c.player[index].numGames - 1].numHoles;
	while (c.player[index].game[c.player[index].numGames - 1].numHoles < 1 || c.player[index].game[c.player[index].numGames - 1].numHoles>18) {
		cout << "Number of holes must be between 1 and 18.\n";
		cout << "Enter number of holes:";
		cin >> c.player[index].game[c.player[index].numGames - 1].numHoles;
	} cout << "Enter score for each hole, with spaces between:";
	for (int k = 0; k < c.player[index].game[c.player[index].numGames - 1].numHoles; k++) {
		cin >> c.player[index].game[c.player[index].numGames - 1].hole[k];
	}
}

  //-----------------------------------------------------------------------------
  //                               addGame
  //-----------------------------------------------------------------------------
  // Modifies: a club structure
  // Adds game data for a new game to a player in the club
  //-----------------------------------------------------------------------------
void addGame(aClub & c) {
	string pSel; 
	int index = 0;
	int playerFound = 0;
	printLogo();
	for (int i = 0; i < c.numPlayers; i++) {
		cout << c.player[i].name << endl;
	}
	cout << "Enter player's name:";
	cin.ignore();
	getline(cin, pSel);
	for (int j = 0; j < c.numPlayers; j++) {
		if (pSel == c.player[j].name) {
			index = j;
			playerFound = 1;
		}}
	if (playerFound == 1) {
		if (c.player[index].numGames == 10) {
			cout << "Error: " << c.player[index].name << " already has the maximum of 10 games.";
		}
		else {
			addGameHelp(c, index);
		}
	} else {
		cout << "No player named " << pSel << " in the club.";
	}
	if (playerFound == 1) {
		cout << "Total score = " << totalScore(c, index, c.player[index].numGames - 1);
		cout << "\nNew game data entered for " << c.player[index].name;
		playerFound = 0;
	}
} // addGame()

  //-----------------------------------------------------------------------------
  //                               sortByName
  //-----------------------------------------------------------------------------
  // Modifies: a club structure
  // Re-orders the club's list of players alphabetically by the players' names
  //-----------------------------------------------------------------------------
void sortByName(aClub & c) {
	printLogo();
	for (int i = 0; i < c.numPlayers - 1; i++) {
		for (int j = i + 1; j < c.numPlayers; j++) {
			if (c.player[i].name.compare(c.player[j].name) > 0) {
				aPlayer k = c.player[i];
				c.player[i] = c.player[j];
				c.player[j] = k;
			}
		}
	}
	cout << "Players sorted by name." << endl;
} // sortByName()

  //-----------------------------------------------------------------------------
  //                               sortByAvg
  //-----------------------------------------------------------------------------
  // Modifies:  a club structure
  // Re-orders the club's list of players by their Average Score, high to low
  //-----------------------------------------------------------------------------
void sortByAvg(aClub & c) {
	printLogo();
	for (int i = 0; i < c.numPlayers-1; i++) {
		for (int j = i + 1; j < c.numPlayers; j++) {
			if (c.player[i].avgScore < c.player[j].avgScore) {
				aPlayer k = c.player[i];
				c.player[i] = c.player[j];
				c.player[j] = k;
			}
		}
	}
	cout << "Players sorted by average." << endl;
} // sortByAvg()

  //-----------------------------------------------------------------------------
  //                               saveData
  //-----------------------------------------------------------------------------
  // Given: a club structure
  // Asks the user for an output file name and, if successfully opened, writes
  // all club data to the file.
  //-----------------------------------------------------------------------------
void saveData(aClub & c) {
	printLogo();
	string filename;
	cout << "Enter output file name (or 'none'):";
	cin >> filename;
	if (filename != "none") {
		ofstream f;
		f.open(filename);
		if (!f.fail()) {
			f << c.numPlayers << " " << c.holeAvg << " " << c.name << endl;
			for (int i = 0; i < c.numPlayers; i++) {
				f << c.player[i].numGames << " " << c.player[i].name << endl;
				for (int j = 0; j < c.player[i].numGames; j++) {
					f << c.player[i].game[j].numHoles << " " << c.player[i].game[j].date << " " << c.player[i].game[j].course << endl;
					for (int k = 0; k < c.player[i].game[j].numHoles; k++) {
						f << c.player[i].game[j].hole[k];
						if (k < c.player[i].game[j].numHoles - 1) {
							f << " ";
						}
						else {
							f << endl;
						}
					}
				}
			}
		}
		else {
			cout << "Failed to open file. Exiting." << endl;
		}
	}
} // saveData()

  //-----------------------------------------------------------------------------
  //                               getMenuOption
  //-----------------------------------------------------------------------------
  // Returns: the valid menu option chosen by the user
  // Displays the main menu and gets the user's option
  //----------------------------------------------------------------------------
int getMenuOption() {
	int option = 0;
	printLogo();
	cout << "1. Add Player \n";
	cout << "2. Drop Player \n";
	cout << "3. Add Game Data for Player \n";
	cout << "4. Print Club Report \n";
	cout << "5. Sort Players by Average \n";
	cout << "6. Sort Players by Name \n";
	cout << EXIT_OPTION << ". Exit \n";

	do {
		cout << " Enter Selection: ";
		cin >> option;
		if (option > EXIT_OPTION || option < 1)
			cout << "Please enter a number between 1 and 7";
	} while (option!=EXIT_OPTION && (option>EXIT_OPTION || option < 1));

	return option;
} // getMenuOption()

  //-----------------------------------------------------------------------------
  //                                 main
  //-----------------------------------------------------------------------------
void main() {
	aClub club;
	startUp(club);
	calcTotals(club);
	calcAvgs(club);
	int option = 0;
	while (option != EXIT_OPTION) {
		option = getMenuOption();
		switch (option) {
		case 1: addPlayer(club);  break;
		case 2: dropPlayer(club); break;
		case 3: addGame(club);    break;
		case 4: printClub(club);  break;
		case 5: sortByAvg(club);  break;
		case 6: sortByName(club); break;
		case EXIT_OPTION: saveData(club);
		}
		system("pause");
	}
} // main()