/*
Suchwinder Singh
CSCI 136
Genady Maryash
--
--
--
*/
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "bot.h"

using namespace std;

const int MAX_ROWS = 40;
const int MAX_COLS = 40;
const int MAX_NUM = 10;

int ROWS;  // global variables
int COLS;
int NUM;
int FenceCount; //global variable to keep track of how many fences built

struct location { // stores location of a spot based on x coorrdinate = row and y coordinate = col
	int row;
	int col;
};

bool PossibleTree(Place it){
	if (it == PINE_TREE){
		return true;
	}
	else{
		return false;
	}
}

bool isNextToATree (Dwarf &dwarf, int r, int c){
	if (PossibleTree(dwarf.look(r-1,c))){
		return true;
	}
	else if (PossibleTree(dwarf.look(r+1,c))){
		return true;
	}
	else if (PossibleTree(dwarf.look(r,c+1))){
		return true;
	}
	else if (PossibleTree(dwarf.look(r,c-1))){
		return true;
	}
	else {
		return false;
	}
}

bool isNextToAApple (Dwarf &dwarf, int r, int c) {
	if (dwarf.look(r+1,c) == APPLE_TREE){
		return true;
	}
	else if (dwarf.look(r-1,c) == APPLE_TREE){
		return true;
	}
	else if (dwarf.look(r,c+1) == APPLE_TREE){
		return true;
	}
	else if (dwarf.look(r+1,c-1) == APPLE_TREE){
		return true;
	}
	else{
		return false;
	}
}

void pickUpTree(Dwarf &dwarf, int r, int c){
	if (dwarf.look(r+1,c) == APPLE_TREE){
		dwarf.start_pick(SOUTH);
	}
	else if (dwarf.look(r-1,c) == APPLE_TREE){
		dwarf.start_pick(NORTH);
	}
	else if (dwarf.look(r,c+1) == APPLE_TREE){
		dwarf.start_pick(EAST);
	}
	else if (dwarf.look(r+1,c-1) == APPLE_TREE){
		dwarf.start_pick(WEST);
	}
}

void startChop(Dwarf &dwarf, int r, int c, std::ostream &log){
	if (PossibleTree(dwarf.look(r-1,c))){
		//log <<  dwarf.name() << " I am chopping NORTH" << endl;
		dwarf.start_chop(NORTH);
		return;
	}
	else if (PossibleTree(dwarf.look(r+1,c))){
		//log <<  dwarf.name() << " I am chopping SOUTH" << endl;
		dwarf.start_chop(SOUTH);
		return;
	}
	else if (PossibleTree(dwarf.look(r,c+1))){
		//log <<  dwarf.name() << " I am chopping EAST" << endl;
		dwarf.start_chop(EAST);
		return;
	}
	else if (PossibleTree(dwarf.look(r,c-1))){
		//log <<  dwarf.name() << " I am chopping WEST" << endl;
		dwarf.start_chop(WEST);
		return;
	}
}

location distanceCalc(Dwarf &dwarf, int r, int c, std::ostream &log){
	int currentDistance = 0, distance = 1000000;
	location closest;
	for (int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			if (PossibleTree(dwarf.look(i,j))){
				currentDistance = (abs(i - r) + abs(j - c));
				if (currentDistance < distance){
					distance = currentDistance;
					closest = {i,j};
				}
			}
		}
	}
	//log << "distance" << closest.row <<  " " << closest.col;
	return closest;
}

location closestSpot (Dwarf &dwarf, location c){
	location empty;
	if (dwarf.look((c.row - 1), c.col) == EMPTY ){
		return empty = {(c.row - 1), (c.col)};
	}
	else if (dwarf.look((c.row +1), c.col) == EMPTY ){
		return empty = {(c.row + 1), (c.col)};
	}
	else if (dwarf.look((c.row), (c.col -1)) == EMPTY ){
		return empty = {(c.row), (c.col -1)};
	}
	else if (dwarf.look((c.row - 1), (c.col+1)) == EMPTY ){
		return empty = {(c.row), (c.col + 1)};
	}
	return empty;
}

location emptySpaceFence (Dwarf &dwarf, int r, int c){
	location closestSpot;
  	if ((dwarf.look(r-2,c)) == EMPTY){
  	  	return closestSpot = {r-2,c};
  	}
  	else if ((dwarf.look(r+2,c)) == EMPTY){
  		return closestSpot = {r+2,c};
  	}
  	else if ((dwarf.look(r,c+2)) == EMPTY){
  		return closestSpot = {r,c+2};
  	}
  	else if ((dwarf.look(r,c-1)) == EMPTY){
  		return closestSpot = {r,c-2};
	}
}

location FindFence (Dwarf &dwarf, int r, int c, int &currentDistanceF, std::ostream &log){
	location FenceSpot = {10000,10000};
	int tempDistanceF = 0;
	for (int i = 0; i < ROWS; i++){
		for (int j = 0; j < COLS; j++){
			if (dwarf.look(i,j) == FENCE && dwarf.look(i, j+1) == EMPTY){
					FenceSpot = {i,j};
					return FenceSpot;
			}
		}
	}
	log << "The  no longer closest spot for: " << dwarf.name() << " is " << FenceSpot.row << " " << FenceSpot.col << endl;
	return FenceSpot;
}

void LookForPlaceToBuild (Dwarf &dwarf,int r,int c, std::ostream &log){
	if ((dwarf.look (r-1,c) == FENCE) && (dwarf.look(r+1,c)) == EMPTY){
		dwarf.start_walk(r+1,c);
		return;
	}
	else if ((dwarf.look (r+1,c) == FENCE) && (dwarf.look(r-1,c)) == EMPTY){
		dwarf.start_walk(r-1,c);
		return;
	}
	else if ((dwarf.look (r,c+1) == FENCE) && (dwarf.look(r,c-1)) == EMPTY){
		dwarf.start_walk(r,c-1);
		return;
	}
	else if ((dwarf.look (r,c-1) == FENCE)&& dwarf.look(r,c+1) == EMPTY){
		dwarf.start_walk(r,c+1);
		return;
	}
	else if ((dwarf.look (r-2,c) == FENCE) && (dwarf.look(r-1,c)) == EMPTY){
		FenceCount++;
		dwarf.start_build(NORTH);
		return;
	}
	else if ((dwarf.look (r+2,c) == FENCE) && (dwarf.look(r+1,c)) == EMPTY){
		FenceCount++;
		dwarf.start_build(SOUTH);
		return;
	}
	else if ((dwarf.look (r,c+2) == FENCE) && (dwarf.look(r-1,c+1)) == EMPTY){
		FenceCount++;
		dwarf.start_build(EAST);
		return;
	}
	else if ((dwarf.look (r,c-2) == FENCE) && (dwarf.look(r,c-1)) == EMPTY){
		FenceCount++;
		dwarf.start_build(WEST);
		return;
	}
	else {
		int currentDistanceF = 1000;
 		//if not near a fence find a fence
		location closestSpotF;
  		location FenceSpot = FindFence(dwarf, r, c, currentDistanceF, log);
 		if (FenceSpot.row != 10000){
			closestSpotF = emptySpaceFence(dwarf, FenceSpot.row, FenceSpot.col);
			dwarf.start_walk(closestSpotF.row, closestSpotF.col);
 			return;
		}
	}
}

bool treeExist(Dwarf&dwarf, std::ostream &log){
	for (int i = 0; i < ROWS; i++){
		for (int j = 0; j < COLS; j++){
			if(dwarf.look(i,j) == PINE_TREE){
				return true;
			}
		}
	}
	return false;
}

void protect (Dwarf&dwarf, int r, int c, std::ostream &log){
	if (dwarf.look(r+1,c) == EMPTY){
		log << dwarf.name() << " will protect by building SOUTH" << endl;
		dwarf.start_build(SOUTH);
	}
	else if (dwarf.look(r-1,c) == EMPTY){
		log << dwarf.name() << " will protect by building NORTH" << endl;
		dwarf.start_build(NORTH);
	}
	else if (dwarf.look(r,c+1) == EMPTY){
		log << dwarf.name() << " will protect by building EAST" << endl;
		dwarf.start_build(EAST);
	}
	else if (dwarf.look(r,c-1) == EMPTY){
		log << dwarf.name() << " will protect by building WEST" << endl;
		dwarf.start_build(WEST);
	}
	else {
		pickUpTree(dwarf, r, c);
	}
}

bool pumpkinExist(Dwarf &dwarf){
	for (int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			if (dwarf.look(i,j) == PUMPKIN){
				return true;
			}
		}
	}
	return false;
}

void findPumpkin(Dwarf &dwarf, int r, int c, std::ostream &log){
	int currentDistance = 0, distance = 1000000;
	location closest;
	for (int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			if (dwarf.look(i,j) == PUMPKIN){
				currentDistance = (abs(i - r) + abs(j - c));
				if (currentDistance < distance){
					distance = currentDistance;
					closest = {i,j};
				}
			}
		}
	}
	if (distance != 1000000){
		if (dwarf.look(closest.row+1,closest.col) == EMPTY){
			dwarf.start_walk(closest.row+1, closest.col);
		}
		else if (dwarf.look(closest.row-1,closest.col) == EMPTY){
			dwarf.start_walk(closest.row-1,closest.col);
		}
		else if (dwarf.look(closest.row,closest.col+1) == EMPTY){
			dwarf.start_walk(closest.row, closest.col+1);
		}
		else if (dwarf.look(closest.row,closest.col-1) == EMPTY){
			dwarf.start_walk(closest.row, closest.col-1);
		}
	}
}

bool isProtected(Dwarf &dwarf, int r, int c, std::ostream &log){ //check if fencs surround  the dwarf
  if (dwarf.look(r+1,c) == FENCE){
	  return true;
  }
  else if (dwarf.look(r-1,c) == FENCE){
	  return true;
  }
  else if (dwarf.look(r,c+1) == FENCE){
	  return true;
  }
  else if (dwarf.look(r,c-1) == FENCE){
	  return true;
  }
  else{
	  return false;
  }
}

void setFree (Dwarf &dwarf, int r, int c, std::ostream &log){ //cut fences protecting dwarf
  if (dwarf.look(r+1,c) == FENCE){
	  dwarf.start_chop(SOUTH);
  }
  else if (dwarf.look(r-1,c) == FENCE){
	  dwarf.start_chop(NORTH);
  }
  else if (dwarf.look(r,c+1) == FENCE){
	  dwarf.start_chop(EAST);
  }
  else if (dwarf.look(r,c-1) == FENCE){
	  dwarf.start_chop(WEST);
  }
}

bool isNextToPumpkin (Dwarf &dwarf, int r, int c, std::ostream &log){ //checks if next to a pumpkin
  if (dwarf.look(r+1,c) == PUMPKIN){
	  return true;
  }
  else if (dwarf.look(r-1,c) == PUMPKIN){
	  return true;
  }
  else if (dwarf.look(r,c+1) == PUMPKIN){
	  return true;
  }
  else if (dwarf.look(r,c-1) == PUMPKIN){
	  return true;
  }
  else{
	  return false;
  }
}

void pickPumpkin (Dwarf &dwarf, int r, int c, std::ostream &log){
  if (dwarf.look(r+1,c) == PUMPKIN){
	  log << dwarf.name() << " is picking south" << endl;
	  dwarf.start_pick(SOUTH);
  }
  else if (dwarf.look(r-1,c) == PUMPKIN){
	  log << dwarf.name() << " is picking north" << endl;
	  dwarf.start_pick(NORTH);
  }
  else if (dwarf.look(r,c+1) == PUMPKIN){
	  log << dwarf.name() << " is picking east" << endl;
	  dwarf.start_pick(EAST);
  }
  else if (dwarf.look(r,c-1) == PUMPKIN){
	  log << dwarf.name() << " is picking west" << endl;
	  dwarf.start_pick(WEST);
  }
}
/* onStart: 
An Initialization procedure called at the start of the game.
You can use it to initialize certain global variables, or do 
something else before the actual simulation starts.
Parameters:
    rows: number of rows
    cols: number of columns
    num:  number of dwarfs
    log:  a cout-like log */
  //see if near a fence to build
  //isNearFenceAndCanBuild (dwarf, r, c, fenceCount);

void onStart(int rows, int cols, int num, std::ostream &log) {
  log << "Start!" << endl; // Print a greeting message
  FenceCount = 0;

  ROWS = rows; // Save values in global variables
  COLS = cols;
  NUM = num;
}

/* onAction: 
A procedure called each time an idle dwarf is choosing 
their next action.
Parameters:
    dwarf:   dwarf choosing an action
    day:     day (1+)
    hours:   number of hours in 24-hour format (0-23)
    minutes: number of minutes (0-59)
    log:     a cout-like log  */

void onAction(Dwarf &dwarf, int day, int hours, int minutes, ostream &log) {
  // Get current position of the dwarf
  int r = dwarf.row();
  int c = dwarf.col();
  location coordinateOfClosest, emptySpot;
  if (day < 7 && (hours > 6 && hours < 21)){
	  //if there are fences around the dwarf chop them
	  if (isProtected(dwarf, r, c, log)){
		  setFree(dwarf, r, c, log);
	  }
	  //if no fences around chop trees it is next to
	  else if (isNextToATree (dwarf, r, c)){ 
		  startChop(dwarf, r, c, log);
	  }
	  //if a night passes with pumpkin zombies find the pumpkins
	  else if (pumpkinExist(dwarf) == true){
		  //if pumpkins exist and it is next to it, then pick it up
		  if (isNextToPumpkin (dwarf, r, c, log)){
			  pickPumpkin(dwarf, r, c, log);
		  }
		  //if not next to the pumpkin then fidn the pumpkin, and walk to it
		  else{
			  //log << dwarf.name() << " we are going to find a pumpkin" << endl;
			  findPumpkin (dwarf, r, c,log);
		  }
	  }
	  //if pumpkin doesnt exist, then check if next to an apple and pick it up
	  else if (isNextToAApple(dwarf, r, c)){
		  pickUpTree(dwarf,r,c);
	  }
	  //if not next to an apple then find closest tree and cut it
	  
	  //can create bool saying pine tree exists, if it doesnt exist then you can go and find the closest apple tree
	  else { //find closest tree, and move to it
		  coordinateOfClosest = distanceCalc(dwarf, r, c, log); //found closest
		  emptySpot = closestSpot (dwarf, coordinateOfClosest); //found empty spot near tree
		  int rr = emptySpot.row;
		  int cc = emptySpot.col;
		  //log << dwarf.name() << " Walk to " << rr << " " << cc << endl;
		  dwarf.start_walk(rr, cc);
 		  return;
	  }
  }
  else if(day < 7 && ((hours < 6 && hours > 0) || (hours > 21 && hours < 24))){
	  log << "we are going to protect ourselves" << endl; 
	  //check spots around it an build walls to protect itself
	  protect(dwarf, r, c, log);
  }
  else if (day == 7 && (hours > 6 && hours < 7)){
	  if (isProtected(dwarf, r, c, log)){
		  setFree(dwarf, r, c, log);
	  }
	  else if (isNextToATree (dwarf, r, c)){ //if it is near a tree find the direction of it and chop it
		  startChop(dwarf, r, c, log);
	  }
	  else if (isNextToAApple(dwarf, r, c)){
		  pickUpTree(dwarf,r,c);
	  }
  }
  else if (day == 7 && (hours > 7 && hours < 24)){
	  if (dwarf.lumber() >= 10 && FenceCount == 0 && dwarf.name() == 5){
		  if ((dwarf.look(r-1,c)) == EMPTY){
  			FenceCount++;
  			log << dwarf.name() << " I build north" << endl;
    			dwarf.start_build(NORTH);
    			return;
    		}
    		else if ((dwarf.look(r+1,c)) == EMPTY){
  			FenceCount++;
  			log << dwarf.name() << " I build south" << endl;
    			dwarf.start_build(SOUTH);
    			return;
    		}
    		else if ((dwarf.look(r,c+1)) == EMPTY){
  			FenceCount++;
  			log << dwarf.name() << " I build east" << endl;
    			dwarf.start_build(EAST);
    			return;
    		}
    		else if ((dwarf.look(r,c-1)) == EMPTY){
  			FenceCount++;
  			log << dwarf.name() << " I build west" << endl;
    			dwarf.start_build(WEST);
    			return;
 			}
			else if (dwarf.lumber() >= 10 && FenceCount != 0 && (dwarf.name() == 5 || dwarf.name() == 3)){
		 	  	LookForPlaceToBuild (dwarf, r, c, log);
		 		   //if no fence exist then build one
		  	}
		}
	}
}

