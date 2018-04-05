#include "Board.h"
#include "SourceData.h"
#include "Tile.h"
#include "Sideline.h"
#include "Vertex.h"
#include "Resources.h"
#include "Edge.h"
#include "Dice.h"
#include "Player.h"
#include "BoardDisplay.h"
#include "UserDisplay.h"
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

// New game, random board
Board::Board(bool seed, int seedVal, string boardLayout) {
	// Tile initialization
	for (int i = 0; i < 19; ++i) {
		auto sharedPtr = make_shared<Tile>(i);
		tiles.emplace_back(sharedPtr);
	}
	// Vertex initialization
	for (int i = 0; i < 54; ++i) {
		auto sharedPtr = make_shared<Vertex>(i);
		vertices.emplace_back(sharedPtr);
	}
	// Edge initialization
	for (int i = 0; i < 72; ++i) {
		auto sharedPtr = make_shared<Edge>(i);
		edges.emplace_back(sharedPtr);
	}
	// Playertur 
	playerTurn = 0;
	// Dice initialization
	diceComponent = make_shared<Dice>(seed, seedVal);
	// Player initialization
	for (int i = 0; i < 4; ++i) {
		char col[4] = {'B', 'R', 'O', 'Y'};
		auto sharedPtr = make_shared<Player>(i,col[i]);
		pPlayers.emplace_back(sharedPtr);
	}
	// UserDisplay initialization
	textBoard = make_shared<UserDisplay>(boardLayout);
	// SourceData initialization
	data = make_shared<SourceData>(seed, seedVal);

	// Associate tiles, etc
	setTileVertices();
	setAdjEdges();
	setAdjVertex();
}

// Provide board, handles saved game files and default files with <board> info only
Board::Board(bool save, bool seed, int seedVal, string saved, string boardLayout) {
	// Tile initialization
	for (int i = 0; i < 19; ++i) {
		auto sharedPtr = make_shared<Tile>(i);
		tiles.emplace_back(sharedPtr);
	}
	// Vertex initialization
	for (int i = 0; i < 54; ++i) {
		auto sharedPtr = make_shared<Vertex>(i);
		vertices.emplace_back(sharedPtr);
	}
	// Edge initialization
	for (int i = 0; i < 72; ++i) {
		auto sharedPtr = make_shared<Edge>(i);
		edges.emplace_back(sharedPtr);
	}
	// Playertur 
	playerTurn = 0;
	// Dice initialization
	diceComponent = make_shared<Dice>(seed, seedVal);
	// Player initialization
	for (int i = 0; i < 4; ++i) {
		char col[4] = {'B', 'R', 'O', 'Y'}; // Assign char colour
		auto sharedPtr = make_shared<Player>(i, col[i]);
		pPlayers.emplace_back(sharedPtr);
	}
	// SourceData initialization
	textBoard = make_shared<UserDisplay>(boardLayout);
	if(!save){
		data = make_shared<SourceData>(saved);}
	// UserDisplay initialization

	// use -1 as placeholder for a board with no geese
	if (save) { // create SourceData obj / Saved game
		ifstream File;
		File.open(saved);

		int saveLines = 7;
		string line;
		for (int i = 0; i < saveLines; ++i) {
			getline(File, line);
			if (i == 0) { // currTurn
				int currTurn;
				istringstream currT{line};
				currT >> currTurn;
				updateCurrTurn(currTurn);
			}
			else if (i < 5) { // builder data
				string check;
				int builderVal;
				int resourceIndex = 0;
				istringstream ssLine{line};
				// Iterate until reach r "roads"
				while(ssLine >> check) {

					if (check == "r") { // indicate hit a road
						break;
					}
					// else read into player resource
					istringstream resource{check};
					resource >> builderVal; 
					// Player number is actual (i - 1)
					updatePlayerResource(resourceIndex, builderVal, i - 1);
					++resourceIndex;
				}
				// continue;
				// Iterate until reach h "housing"
				while(ssLine >> check) { // Read in the roads
					if (check == "h") {
						break;
					}
					int road;
					istringstream pathwayVal{check};
					pathwayVal >> road;
					updatePlayerRoad(road, i - 1);
				}
				// continue;
				// Iterate until the end of the builderDataLine
				while(ssLine >> check) { // read in the pair of player vertices
					// check should be a int, next ssLine >> check should be string
					int address;
					int level;
					istringstream vertex{check};
					vertex >> address; // address of player B,H,T
					cout<<address<<" a"<<endl;
					ssLine >> check; // residence LEVEL
					level = convertLevel(check);
					cout<<level<<" l"<<endl;
					updatePlayerVertex(address, level+1, i - 1);
				}
			}
			else if (i == 5) { // hit the board
				// Construct SourceData obj
				data = make_shared<SourceData>(save, line);
				data->setSaved(true);

			}
			else { // hit the geese, i == 6
				// IF -1, means no geese (0 - 18)
				int geeseLocation;
				istringstream ssLine{line};
				ssLine >> geeseLocation;
				cout<<geeseLocation<<"GEESE"<<endl;
				if(geeseLocation==-1)
				{

				}
				
				
				if (geeseLocation != -1) {
					for (int i = 0; i < 19; ++i)
					{
						tiles[i]->setGeese(false);
					}
					tiles[geeseLocation]->setGeese(true);
				}
			}
		}
	}
	// Associate tiles, etc
	setTileVertices();
	setAdjEdges();
	setAdjVertex();

	cout << "reached " << endl;
}

int Board::convertLevel(string level) {
	if (level == "B") {
		return 0;
	}
	else if (level == "H") {
		return 1;

	}
	else { // level == T
		return 2;
	}
}

// passing a reference to Board object
void Board::printBoard() {
	textBoard->printBoardDisplay(*this);
}

int Board::diceRoll(bool isFair) {
	int roll;
	if (isFair) {
		roll = diceComponent->rollFairDice();
		return roll;
	}
	// Else
	roll = diceComponent->rollLoadedDice();
	return roll;
}

void Board::next() { // Go to next players turn
	// Increment playerTurn to keep track
	playerTurn = playerTurn == 3 ? 0 : playerTurn + 1;
}

int Board::getPlayerTurn() {
	return playerTurn;
}

// Should not be called until after all pieces of game
//. board have been set up
void Board::updateResources(int diceRoll) {
	// Iterate through tiles
	int tileSize = tiles.size();
	for (int i = 0; i < tileSize; ++i) {
		if (tiles[i]->getTileVal() == diceRoll) {

			cout << "tile val: " << tiles[i]->getTileVal() << endl;
			// Call tile distributeResource function
			if (!tiles[i]->existGeese()){
				tiles[i]->DistributeResource();
			}
		}
	}
}

// Function for starting a brand new game
// Option to generate a random board or a specified Board
void Board::updateBoard(){
	data->NotifyBoard();
}


// here int parameters should NEVER be out of the vertex range
shared_ptr<Tile> Board::getTile(int t) {
	return tiles[t];
}
shared_ptr<Vertex> Board::getVertex(int v) {
	return vertices[v];
}
shared_ptr<Edge> Board::getEdge(int e) {
	return edges[e];
}
shared_ptr<Player> Board::getPlayer(int p) {
	return pPlayers[p];
}



void Board::setTileVertices()
{
	ifstream ifile;
	ifile.open("TilesVertices.txt");
	int a,b,c,d,e,f=0;
	int tileSize = tiles.size();
	for(int i=0;i<tileSize;i++)
	{
		std::string line;
		std::getline(ifile,line);
		std::istringstream iss(line);
		std::vector<shared_ptr<Vertex>> v;
		if(iss>>a>>b>>c>>d>>e>>f)
		{
			v.push_back(vertices[a]);
			v.push_back(vertices[b]);
			v.push_back(vertices[c]);
			v.push_back(vertices[d]);
			v.push_back(vertices[e]);
			v.push_back(vertices[f]);
		}
		tiles[i]->setVertices(v);

	}
}

void Board::setAdjEdges()
{
	ifstream ifile;
	ifile.open("Edges.txt");
	int a,b,c=0;
	int verticeSize = vertices.size();
	for (int i = 0; i < verticeSize; ++i)
	{
		std::string line;
		std::getline(ifile,line);
		std::istringstream iss(line);
		std::vector<shared_ptr<Edge>> e;
		if(!(iss>>a>>b>>c))
		{
			e.push_back(edges[a]);
			e.push_back(edges[b]);
		}
		else
		{
			e.push_back(edges[a]);
			e.push_back(edges[b]);
			e.push_back(edges[c]);
		}
		vertices[i]->setAdjEdges(e);
	}
}

void Board::setAdjVertex()
{
	ifstream ifile;
	ifile.open("Vertices.txt");
	int a,b=0;
	int edgeSize = edges.size();
	for (int i = 0; i < edgeSize; ++i)
	{
		std::string line;
		std::getline(ifile,line);
		std::istringstream iss(line);
		std::vector<std::shared_ptr<Vertex>> av;
		if((iss>>a>>b))
		{
			av.push_back(vertices[a]);
			av.push_back(vertices[b]);
		}
		
		edges[i]->setAdjVertex(av);
	}
}

void Board::updateCurrTurn(int turns){
	this->playerTurn = turns;
}
void Board::updatePlayerResource(int resource, int amount, int players){
	(this->pPlayers[players])->getResources(resource, amount);
}
void Board::updatePlayerVertex(int address, int level, int players){
	shared_ptr<Player> Pplay= pPlayers[players];
	shared_ptr<Vertex> PVert = vertices[address];
	
	for (int i = 0; i < level; i++){
		PVert->levelup();
	}
	PVert->setOwner(players, Pplay);
	Pplay->setResidence(PVert);
	cout<<vertices[address]->getOwnerID()<<"id"<<endl;
}
void Board::updatePlayerRoad(int path, int players){
	shared_ptr<Player> Pplay = this->pPlayers[players];
	shared_ptr<Edge> PEdge = this->edges[path];
	PEdge->setOwner(players, Pplay);
}

// Attach board method
void Board::attachBoard(std::shared_ptr<Board> b) {
	data->attachBoard(b);
	int playerSize = pPlayers.size();
	for (int i = 0; i < playerSize; ++i) {
		pPlayers[i]->attachBoard(b);
	}
}

// main.cc methods
void Board::chosestarting(){
	printBoard(); // Fine because we're not mutating the Board
	
	for (int i = 0; i < 4; i++){
		
		shared_ptr<Player> a = pPlayers[i];
		int aa;
		while(true){
			cout << "Player " << i << "!  Choose a Vertex to Build Your First Building" << endl;
			cout << "> ";
			cin >> aa;
			bool adj = true;
			int size = vertices[aa]->getsize();
			for (int j = 0; j < size; j++){
				shared_ptr<Edge> v = vertices[aa]->getEdge(j);
				for (int k = 0; k < 2; k++){
					if (v->getVertex(k)->getOwnerID() != -1){
						adj = false;
					}
				}
			}
			if (aa <= 54 and aa >= 0 and vertices[aa]->getOwnerID() == -1 and adj){
				updatePlayerVertex(aa, 1, i);
				break;
			}
			cout << "You cannot build here." << endl;
		} // Basement first

		// while(true){
		// 	cout << "Choose a Edge to Build Your First Road" << endl;
		// 	cin >> aa;
		// 	bool adj = false;
		// 	for (int j = 0; j < 2; j++){
		// 		shared_ptr<Vertex> v = edges[aa]->getVertex(j);
		// 		if (v->getOwnerID() == i){
		// 			adj = true;
		// 		}
		// 	}
		// 	if (aa <= 71 and aa >= 0 and edges[aa]->getOwnerID() == -1 and adj){
		// 		updatePlayerRoad(aa, i);

		// 		printBoard(); // Fine because we're not mutating the Board
		// 		break;
		// 	}
		// 	else{
		// 		cout << "You cannot build here." << endl;
		// 	}
		// }

	}
	for (int i = 3; i >=0 ; --i){
		
		shared_ptr<Player> a = pPlayers[i];
		int aa;
		while(true){
			cout << "Player " << i << "!  Choose a Vertex to Build Your Second Building" << endl;
			cout << "> ";
			cin >> aa;
			bool adj = true;
			int size = vertices[aa]->getsize();
			for (int j = 0; j < size; j++){
				shared_ptr<Edge> v = vertices[aa]->getEdge(j);
				for (int k = 0; k < 2; k++){
					if (v->getVertex(k)->getOwnerID() != -1){
						adj = false;
					}
				}
			}
			if (aa <= 54 and aa >= 0 and vertices[aa]->getOwnerID() == -1 and adj){
				updatePlayerVertex(aa, 1, i);

				break;
			}
			cout << "You cannot build here." << endl;
		} // Basement first

		// while(true){
		// 	cout << "Choose a Edge to Build Your First Road" << endl;
		// 	cin >> aa;
		// 	bool adj = false;
		// 	for (int j = 0; j < 2; j++){
		// 		shared_ptr<Vertex> v = edges[aa]->getVertex(j);
		// 		if (v->getOwnerID() == i){
		// 			adj = true;
		// 		}
		// 	}
		// 	if (aa <= 71 and aa >= 0 and edges[aa]->getOwnerID() == -1 and adj){
		// 		updatePlayerRoad(aa, i);

		// 		printBoard(); // Fine because we're not mutating the Board
		// 		break;
		// 	}
		// 	else{
		// 		cout << "You cannot build here." << endl;
		// 	}
		// }

	}

}

void Board::playturn(int Players){
	shared_ptr<Player> p = getPlayer(Players);

	// printBoard etc...
	cout << "Builder " << p->convertColor() << "'s " << "turn." << endl;

	cout << "Choose Your Dice" << endl;
	string a;
	while (a != "roll"){
		cout << "> ";
		cin >> a;
		if (a != "roll"){
			if (a != "fair" and a != "load") {
				cout << "Invalid command" << endl;
				continue;
			} // Else can assign a to b
			bool isload = a == "fair" ? false : true;
			p->setDice(isload);
		}
		// Else (a) is == roll
	}

	// use current Players loadedDice bool field as a check
	int roll;
	if (!(p->isLoad())){
		roll = this->diceRoll(true); // isFair true
	}
	else{ // assume roll is Loaded
		roll = this->diceRoll(false); // isFair false
	}

	cout << "The Dice Roll is: " << roll << endl;
	if (roll == 7){
		this->moveGeese();
	}
	else{
		this->updateResources(roll);
	}
	string d;
	while (d != "next"){
		cout << "> ";
		cin >> d;
		if (d == "board"){
			printBoard();
		}
		else if (d == "status"){
			for (int i = 0; i < 4; i++){
				cout << pPlayers[i]->convertColor() <<" has " << pPlayers[i]->playerStatuses() << " building points, " <<pPlayers[i]->getRIndex(0)<<" brick, "<<pPlayers[i]->getRIndex(1)<<" energy, "<<pPlayers[i]->getRIndex(2)<<" glass, "<<pPlayers[i]->getRIndex(3)<<" heat, and "<<pPlayers[i]->getRIndex(4)<<" WiFi."<<endl;
// 				cout<< <colour> has <numPoints> building points, <numBrick> brick, <numEnergy> energy,
// <numGlass> glass, <numHeat> heat, and <numWiFi> WiFi.

			}
		}
		else if (d == "residences"){
			cout << pPlayers[Players]->playerResidence() << endl;
		}
		else if (d == "build-road"){
			int e;
			cin >> e;
			bool f = pPlayers[Players]->buildroad(e);
			if (f){
				cout << "Successfully Built" << endl;
			}
			else{
				cout << "Error" << endl;
			}
		}
		else if (d == "build-res"){
			int e;
			cin >> e;
			bool f = pPlayers[Players]->buildres(e);
			if (f){
				cout << "Successfully Built" << endl;

			}
			else{
				cout << "Error" << endl;
			}
		}
		else if (d == "improve"){
			int e;
			cin >> e;
			bool f = pPlayers[Players]->improve(e);
			if (f){
				cout << "Successfully Improved" << endl;
			}
			else{
				cout << "Error" << endl;
			}
		}
		else if (d == "trade"){
			string x[2];
			string ress[2];
			int color[2];
			int res[2];
			cin >> x[1] >> ress[0] >> ress[1];
			if (x[1] == "Blue") color[1] = 0;
			else if (x[1] == "Red") color[1] = 1;
			else if (x[1] == "Orange") color[1] = 2;
			else if (x[1] == "Yellow") color[1] = 3;
			for (int i = 0; i < 2; i++){
				if (ress[i] == "Brick") res[i] = 0;
				else if (ress[i] == "Energy") res[i] = 1;
				else if (ress[i] == "Glass") res[i] = 2;
				else if (ress[i] == "Heat") res[i] = 3;
				else if (ress[i] == "Wifi") res[i] = 4;
			}
			color[0] = playerTurn;
			if (color[0] == 0) x[0] = "Blue";
			else if (color[0] == 1) x[0] = "Red";
			else if (color[0] == 2) x[0] = "Orange";
			else if (color[0] == 3) x[0] = "Yellow";
			cout << x[0] << " offers " << x[1] << " one " << ress[0] << " for one " << ress[1] << "." << endl;
			cout << "Does " << x[1] << " accept this offer?" << endl;
			string u;
			cin >> u;
			if (u == "yes"){
				bool q = pPlayers[Players]->trade(playerTurn, res[0], res[1]);
				if (q){
					cout << "Trade Successful" << endl;
				}
				else{
					cout << "Trade Error, Not Enough resources" << endl;
				}
			}
		}
		else if (d == "help"){
			cout << "Valid commands:" << endl;
			cout << "board" << endl;
			cout << "status" << endl;
			cout << "residences" << endl;
			cout << "build-road <path#>" << endl;
			cout << "build-res <housing#>" << endl;
			cout << "improve <housing#>" << endl;
			cout << "trade <colour> <give> <take>" << endl;
			cout << "next" << endl;
			cout << "save <file>" << endl;
			cout << "help" << endl;
		}
		else if (d == "save") {
			// next cin will be a string of file unless cin.eof();
			string saveFile;
			cin >> saveFile;

			cout << saveFile << endl;
			saveGame(saveFile);
			cout << "fuck" << endl;
		}
		else if (cin.eof()) { // sudden eof was given
			cout << "Backup Saved" << endl;
			saveGame("backup.sv");
		}
		else { // invalid command
			cout << "Invalid command" << endl;
		}
	}
}
// SAVE GAME INFO INTO <FILE>
// helper functions
string Board::convertResToString(int level) {
	if (level == 0) {
		return "B";
	}
	else if (level == 1) {
		return "H";
	}
	else { // level == 2
		return "T";
	}
}
string Board::convertResource(Resources re) {
	string r;
	Resources park = Resources::PARK;
	Resources brick = Resources::BRICK;
	Resources energy = Resources::ENERGY;
	Resources glass = Resources::GLASS;
	Resources heat = Resources::HEAT;

	if (re == park) {
		r = "5";
	}
	else if (re == brick) {
		r = "0";
	}
	else if (re == energy) {
		r = "1";
	}
	else if (re == glass) {
		r = "2";
	}
	else if (re == heat) {
		r = "3";
	}
	else { // WIFI
		r = "4";
	}
	return r;
}

string Board::builderData(int player) {
	string resource;
	string roads;
	string residence;

	int diffResources = 5;
	for (int i = 0; i < diffResources; ++i) {
		int resoi = pPlayers[player]->getRIndex(i);
		stringstream ss;
		ss << resoi;
		resource += ss.str() + " ";
	}

	int iter = pPlayers[player]->ownRes();

	roads += "r ";
	int edgeSize = edges.size();
	for (int i = 0; i < edgeSize; ++i) {
		if (edges[i]->getOwnerID() == player) {
			stringstream os;
			os << i;
			roads += os.str() + " ";
		}
	}

	residence += "h ";
	string res;
	int initialSpace = 0;
	for (int i = 0; i < iter; ++i) {
		if (initialSpace) {
			res += " ";
		}
		shared_ptr<Vertex> v = pPlayers[player]->getVIndex(i);
		stringstream ss;
		ss << v->getAddress();
		res += ss.str() + " ";
		res += convertResToString(v->getlevel());
		++initialSpace;
	}
	residence += res;
	string pInfo = resource + roads + residence;
	return pInfo;
}

void Board::saveGame(string file) {
	cin.ignore();

	cout << "hi" << endl;
	string currTurn;
	string builder01;
	string builder02;
	string builder03;
	string builder04;
	string board;
	string geese;

	stringstream o;
	o << playerTurn;
	currTurn = o.str();

	builder01 = builderData(0);
	builder02 = builderData(1);
	builder03 = builderData(2);
	builder04 = builderData(3);

	int tileSize = 19;
	for (int i = 0; i < tileSize; ++i) { // For <board>
		cout << "work here hoe..." << endl;

		board += convertResource(tiles[i]->getResource());
		cout << convertResource(tiles[i]->getResource()) << endl;

		stringstream ss;
		ss << tiles[i]->getTileVal();
		board += (" " + ss.str() + " ");
		cout << "ss.str(): " << ss.str() << endl;
	}
	for (int j = 0; j < tileSize; ++j) {
		if (tiles[j]->existGeese()) {
			stringstream g;
			g << j;
			geese = g.str();
			break;
		}
		// else geese = -1
		geese = "-1";
	}
	ofstream toFile;
	toFile.open(file);
	toFile << currTurn << endl;
	toFile << builder01 << endl;
	toFile << builder02 << endl;
	toFile << builder03 << endl;
	toFile << builder04 << endl;
	toFile << board << endl;
	toFile << geese << endl;
}

void Board::moveGeese(){
	for (int i = 0; i < 4; i++){
		int count = 0;
		for (int j = 0; j < 5; j++){
			count += pPlayers[i]->getRIndex(j);
		}
		if (count >= 10){
			for (int k = 0; k < count/2; k++){
				int numb = rand()%4;
				if (pPlayers[i]->getRIndex(numb) >= 1){
					pPlayers[i]->getResources(numb, -1);
				}
				else{
					continue;
				}
			}
		}
	}
	int current;
	for (int i = 0; i < 19; i++){
		if (tiles[i]->existGeese()){
			current = i;
		}
	}
	int a;
	while (true){
		cout << "Choose a Tile to Move the Geese" << endl;
		cout << "curr: " <<  current << endl;
		cin >> a;
		if (a == current){
			cout << "Invalid Selection Go Again" << endl;
		}
		else{
			break;
		}
	}
	tiles[current]->setGeese(false);
	tiles[a]->setGeese(true);
	
	string possiblePlayers[4];
	for (int i = 0; i < 6; i++){
		shared_ptr<Vertex> vert = tiles[a]->getVertices(i);
		int ownerid = vert->getOwnerID();
		if (ownerid >= 0){
			if (ownerid == 0) possiblePlayers[0] = "Blue";
			else if (ownerid == 1) possiblePlayers[1] = "Red";
			else if (ownerid == 2) possiblePlayers[2] = "Orange";
			else if (ownerid == 3) possiblePlayers[3] = "Yellow";
		}
	}
	string choice;
	int choices;
	while (true){
		cout<<"choose\n"<<endl;
		cin.ignore();
		getline(cin,choice);
		if (choice == possiblePlayers[playerTurn]){
			cout << "Invalid, you can't choose yourself" << endl;
		}
		else{
			break;
		}
	}
	if (choice == "Blue") choices = 0;
	else if (choice == "Red") choices = 1;
	else if (choice == "Orange") choices = 2;
	else if (choice == "Yellow") choices = 3;
	int resourcetaken;
	while (true){
		cout<<"here"<<endl;
		bool allz=true;
		for (int i = 0; i < 5; ++i)
		{
			if (pPlayers[choices]->getRIndex(i)>0)
			{
				allz=false;
			}
		}
		if (allz)
		{
			break;
		}
		resourcetaken = rand()%4;
		// cout<<resourcetaken<<endl;
		cout<<pPlayers[choices]->getRIndex(resourcetaken)<<endl;
		if (pPlayers[choices]->getRIndex(resourcetaken) > 0){
			break;
		}
	}
	pPlayers[choices]->getResources(resourcetaken, -1);
	pPlayers[playerTurn]->getResources(resourcetaken, 1);
}
	
			
