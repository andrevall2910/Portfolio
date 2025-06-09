#ifndef INC_FLOODFILL_H_
#define INC_FLOODFILL_H_

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

#include "api.h"
#include "eventLoop.h"

int mouse_x=0; //calculation
int mouse_y=0;  //calculation
int place1=0; //placeholder
int place2=0; //placeholder
int dir=0;  //calculation
int turn=0;  //calculation
//int target=0; //constant
int MAZE_SIZE=16; //constant
int distance[4]; //outside data
int num=14; //mouse distance
int z=0;  //condition for queue
int min=0;  //target square distance
//int max=0;
int mouse_x_prev = 0;
int mouse_y_prev = 0;

void markWall(int x, int y, int dir, bool hasWall);
void floodfill();
void enqueue(int element1, int element2);
void dequeue();
void trackPath() ;
int min4(int a, int b, int c, int d);
bool hasKnownWall(int x, int y, int a);
void checkAdjacentSquares(int x, int y) ;
void flood();
void checkWalls(int x, int y);
void initWall() ;
void markWall(int x, int y, int dir, bool hasWall);


// this is the master map / array of where the mouse would like to go
// it starts a 14 and heads toward 0 as part of its program
unsigned char maze[16][16] ={
        { 14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14 },
        { 13, 12, 11, 10,  9, 8, 7, 6, 6, 7, 8,  9, 10, 11, 12, 13 },
        { 12, 11, 10,  9,  8, 7, 6, 5, 5, 6, 7,  8,  9, 10, 11, 12 },
        { 11, 10,  9,  8,  7, 6, 5, 4, 4, 5, 6,  7,  8,  9, 10, 11 },
        { 10,  9,  8,  7,  6, 5, 4, 3, 3, 4, 5,  6,  7,  8,  9, 10 },
        {  9,  8,  7,  6,  5, 4, 3, 2, 2, 3, 4,  5,  6,  7,  8,  9 },
        {  8,  7,  6,  5,  4, 3, 2, 1, 1, 2, 3,  4,  5,  6,  7,  8 },
        {  7,  6,  5,  4,  3, 2, 1, 0, 0, 1, 2,  3,  4,  5,  6,  7 },
        {  7,  6,  5,  4,  3, 2, 1, 0, 0, 1, 2,  3,  4,  5,  6,  7 },
        {  8,  7,  6,  5,  4, 3, 2, 1, 1, 2, 3,  4,  5,  6,  7,  8 },
        {  9,  8,  7,  6,  5, 4, 3, 2, 2, 3, 4,  5,  6,  7,  8,  9 },
        { 10,  9,  8,  7,  6, 5, 4, 3, 3, 4, 5,  6,  7,  8,  9, 10 },
        { 11, 10,  9,  8,  7, 6, 5, 4, 4, 5, 6,  7,  8,  9, 10, 11 },
        { 12, 11, 10,  9,  8, 7, 6, 5, 5, 6, 7,  8,  9, 10, 11, 12 },
        { 13, 12, 11, 10,  9, 8, 7, 6, 6, 7, 8,  9, 10, 11, 12, 13 },
        { 14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14 }
    };

// this is where we check if we have been to this square if true then no need to re scan the walls
unsigned char check[16][16];
unsigned char marked[16][16];

// this is where we keep track of the walls and assign them numbers according to flood fill and this helps us direct our path basted on the master maze map
char walls[16][16];

int queuex[5000];
int queuey[5000];
int front = -1;
int rear = -1;

int count=0;


void initFloodfill() {
	initWall(); //setting up the borders and the right wall on the starting square
	for (int x = 0; x < MAZE_SIZE; x++) {
		for (int y = 0; y < MAZE_SIZE; y++) {
			marked[x][y] = 0;
			check[x][y] = 0;
		}
	}
}

void resetFloodfill() {
	for (int x = 0; x < MAZE_SIZE; x++) {
		for (int y = 0; y < MAZE_SIZE; y++) {
			marked[x][y] = 0;
		}
	}

	mouse_x=0;
	mouse_y=0;
	dir=0;
	turn=0;
	num=maze[0][0];
	mouse_x_prev = 0;
	mouse_y_prev = 0;

	z=0;
	front = -1;
	rear = -1;
	count=0;
}

void floodfill() {

    while (maze[mouse_x][mouse_y]!=0) { //until the distance equals zero

    	if (mode == IDLE) {
    		API_Stop();
    		return;
    	}


		//lets try debugging the mouse reverse
    	if (mouse_x != mouse_x_prev || mouse_y != mouse_y_prev)
    		printf("Mouse coordinates: X: %d, Y: %d\n", mouse_x, mouse_y);

    	mouse_x_prev = mouse_x;
    	mouse_y_prev = mouse_y;
		//printf("Direction variable: %d\n", dir);


		// main logic
		if (!check[mouse_x][mouse_y]){ //updates walls if it is a new square
		  checkWalls(mouse_x,mouse_y);
		}

		if (mode == IDLE) {
			API_Stop();
			return;
		}

		checkAdjacentSquares(mouse_x,mouse_y); //finds the shortest distance from adjacent squares

        if (min<num){ //only move if the distance of nearby squares are less than the current one

          if ((turn-dir)==1){
              API_turnRight();
          }
          else if((turn-dir)==-1){
              API_turnLeft();
          }
          else if((turn-dir)==3){
              API_turnLeft();
          }
          else if((turn-dir)==-3){
              API_turnRight();
          }
          else if((turn-dir)==2){
              API_turnRight();
              API_turnRight();
          }
          else if((turn-dir)==-2){
              API_turnRight();
              API_turnRight();
          }

          num=min;

          dir=turn;

          if (mode == IDLE) {
			API_Stop();
			return;
		}

          API_moveForward();

          trackPath(); //updates mouse coordinates
        } else {
          flood(); //update square distances
          num=maze[mouse_x][mouse_y];
          if (count>1000){
          	break;
        }
        count++;
        API_Stop();
      }
    }
    API_Stop();
    if (maze[mouse_x][mouse_y]== 0)
    	printf("Finished maze!\n");
    if (count>1000)
		printf("Got confused.\n");
}

void floodfill_nowrite() {

    while (maze[mouse_x][mouse_y]!=0) { //until the distance equals zero

    	if (mode == IDLE) {
    		API_Stop();
    		return;
    	}

		checkAdjacentSquares(mouse_x,mouse_y); //finds the shortest distance from adjacent squares

        if (min<num){ //only move if the distance of nearby squares are less than the current one

          if ((turn-dir)==1){
              API_turnRight();
          }
          else if((turn-dir)==-1){
              API_turnLeft();
          }
          else if((turn-dir)==3){
              API_turnLeft();
          }
          else if((turn-dir)==-3){
              API_turnRight();
          }
          else if((turn-dir)==2){
              API_turnRight();
              API_turnRight();
          }
          else if((turn-dir)==-2){
              API_turnRight();
              API_turnRight();
          }

          num=min;

          dir=turn;

          if (mode == IDLE) {
			API_Stop();
			return;
		}

          API_moveForward();

          trackPath(); //updates mouse coordinates
        } else {
          flood(); //update square distances
          num=maze[mouse_x][mouse_y];
          if (count>1000){
        	  return;
        }
        count++;
        API_Stop();
      }
    }
    API_Queue();
    API_Stop();
}

void Tremaux() {

	int count = 0;

	while (count++ < 100) { ///////////////////////////////////////////////////////////////

		if (mode == IDLE) {
			API_Stop();
			return;
		}

		if (mouse_x != mouse_x_prev || mouse_y != mouse_y_prev)
			printf("Mouse coordinates: X: %d, Y: %d\n", mouse_x, mouse_y);

		mouse_x_prev = mouse_x;
		mouse_y_prev = mouse_y;

		if (!check[mouse_x][mouse_y]){ //updates walls if it is a new square
		  checkWalls(mouse_x,mouse_y);
		}


		// Check if in an intersection
		int w = walls[mouse_x][mouse_y];

		int w_front = w & (1 << dir);
		int w_left  = w & (1 << ((dir + 3) % 4));
		int w_right = w & (1 << ((dir + 1) % 4));

		if (w && !(w & (w-1))) { // Is intersection if only has one wall (check if only one bit is set)

			// Mark the entrance we just came from
			switch(dir) {
			case 0: // Came from south
				marked[mouse_x][mouse_y] += 1 << 4;
				break;
			case 1: // Came from west
				marked[mouse_x][mouse_y] += 1 << 6;
				break;
			case 2: // Came north
				marked[mouse_x][mouse_y] += 1 << 0;
				break;
			case 3: // Came from east
				marked[mouse_x][mouse_y] += 1 << 2;
				break;
			}



			// Find lowest marked direction out of intersection
			int _mark = marked[mouse_x][mouse_y];

			int n_mark = (_mark&0b00000011) >> 0;
			int e_mark = (_mark&0b00001100) >> 2;
			int s_mark = (_mark&0b00110000) >> 4;
			int w_mark = (_mark&0b11000000) >> 6;

			int lowest;

			if (!hasKnownWall(mouse_x, mouse_y, 0)) {
				lowest = n_mark;
				turn = 0;
			} else {
				lowest = e_mark;
				turn = 1;
			}

			if (e_mark < lowest && !hasKnownWall(mouse_x, mouse_y, 1)) {
				turn = 1;
				lowest = e_mark;
			}
			if (s_mark < lowest && !hasKnownWall(mouse_x, mouse_y, 1)) {
				turn = 2;
				lowest = s_mark;
			}
			if (w_mark < lowest && !hasKnownWall(mouse_x, mouse_y, 1)) {
				turn = 3;
				lowest = w_mark;
			}

			printf("At junction: %d, %d, %d, %d\n", n_mark, e_mark, s_mark, w_mark);
			printf("Going %d\n", turn);

			// Exit condition
			if (n_mark >= 2 && e_mark >= 2 && s_mark >= 2 && w_mark >= 2) {
				break;
			}


			switch(turn) {
			case 0: // Go north
				marked[mouse_x][mouse_y] += 1 << 0;
				break;
			case 1: // Go east
				marked[mouse_x][mouse_y] += 1 << 2;
				break;
			case 2: // Go south
				marked[mouse_x][mouse_y] += 1 << 4;
				break;
			case 3: // Go west
				marked[mouse_x][mouse_y] += 1 << 6;
				break;
			}

			_mark = marked[mouse_x][mouse_y];

			n_mark = (_mark&0b00000011) >> 0;
			e_mark = (_mark&0b00001100) >> 2;
			s_mark = (_mark&0b00110000) >> 4;
			w_mark = (_mark&0b11000000) >> 6;

			printf("Exiting, marked as: %d, %d, %d, %d\n", n_mark, e_mark, s_mark, w_mark);

		} else if (w_front) {
			printf("Wall info: (%d) %d, %d, %d\n", w, w_front, w_left, w_right);
			if (w_left && !w_right) {
				turn = ((dir + 1) % 4);
			}
			if (w_right && !w_left) {
				turn = ((dir + 3) % 4);
			}
			if (w_right && w_left) {
				turn = ((dir + 2) % 4);
			}
		} else {
			turn = dir;
			printf("Skipped stuff.\n");
		}


		if ((turn-dir)==1){
			API_turnRight();
		}
		else if((turn-dir)==-1){
			API_turnLeft();
		}
		else if((turn-dir)==3){
			API_turnLeft();
		}
		else if((turn-dir)==-3){
			API_turnRight();
		}
		else if((turn-dir)==2){
			API_turnRight();
			API_turnRight();
		}
		else if((turn-dir)==-2){
			API_turnRight();
			API_turnRight();
		}

		dir=turn;

		if (mode == IDLE) {
			API_Stop();
			return;
		}

		API_moveForward();

		trackPath();
	}
}

// queue elements for processsing
void enqueue(int element1, int element2) {
    z=0;
    if (front == -1) {
        front = 0;
    }
    rear++;
    queuex[rear] = element1;
    queuey[rear] = element2;
    return;
}

// remove elements from processsing
void dequeue() {
    if (front == -1) {
        //printf("Queue is empty");
        z=-1;
        return;  // Return a default value when the queue is empty
    }
    else{
      place1 = queuex[front];
      place2 = queuey[front];
      front++;
      if (front>rear){

        front=-1;
        rear=-1;
      }
      return;  // Return a value to cover all control paths
    }
}


// this keeps track of the mouse position as following
// mouse starts in the "bottom left" corrner so if dir is unmodified the mouse moves foward etc etc so the go on as follows
// 0 = positive y direction (north)
// 1 = positive x direction (east)
// 2 = negative y direction (south)
// 3 = negative x direction (west)
void trackPath() { //updates mouse position

  //printf("trackpath dir : %d\n", dir);
  if (dir==0){
    mouse_y++;
  }
  else if (dir==1){
    mouse_x++;
  }
  else if (dir==2){
    mouse_y--;
  }
  else if (dir==3){
    mouse_x--;
  }
}

// finds the minimum of the directions as part of checkAdjacentSquares
int min4(int a, int b, int c, int d) {
    int minVal = a;
    int turnDir = 0; // Assume initial direction is 0

    if (b < minVal) {
        minVal = b;
        turnDir = 1;
    }
    if (c < minVal) {
        minVal = c;
        turnDir = 2;
    }
    if (d < minVal) {
        minVal = d;
        turnDir = 3;
    }

    turn = turnDir;
    return minVal;
}

/*int manhattanDist(int x1, int y1, int x2, int y2) {
  return abs(x1-x2) + abs(y1-y2);
}*/

// returns true or false based on if wall has been previously marked
bool hasKnownWall(int x, int y, int a) {
  return walls[x][y] & (1 << a);
}

//looks for path by checking next squares in the path like so:

// mouse_y and mouse_x are passed into x and y so we need to ensure this new position x + 1 or y + 1 isnt out of bounds
// if it is then that position is invalid and we cant move to it
// same for if that particular direction has a wall
void checkAdjacentSquares(int x, int y) {
  int x1;
  int y1;

  if (y + 1 < MAZE_SIZE) {
    y1=y+1;
    // 5/20/25 we should use x and y since those are the values being based to this function anyway:
    // if (hasKnownWall(mouse_x,mouse_y,0)){ //if wall is in that direction
    //   distance[0]=256;
    // }
    if (hasKnownWall(x,y,0)){ //if wall is in that direction
      distance[0]=256;
    }
    else{ //distance is taken if within boundaries and there is no wall in between
      distance[0]=maze[x][y1];
    }
  }
  else{ // if square is outside boundaries
    distance[0]=256;
  }

  if (x + 1 < MAZE_SIZE) {
    x1=x+1;
    if (hasKnownWall(x,y,1)){
      distance[1]=256;
    }
    else{
      distance[1]=maze[x1][y];
    }
  }
  else{
    distance[1]=256;
  }

  if (y - 1 >= 0) {
    y1=y-1;
    if (hasKnownWall(x,y,2)){
      distance[2]= 256;
    }
    else{
      distance[2]=maze[x][y1];
    }
  }
  else{
    distance[2]=256;
  }

  if (x - 1 >= 0) {
    x1=x-1;
    if (hasKnownWall(x,y,3)){
      distance[3]=256;

    }
    else{
      distance[3]=maze[x1][y];
    }
  }
  else{
    distance[3]=256;
  }

  min=min4(distance[0], distance[1], distance[2], distance[3]);
}

// breadth first search logic
// updates the map using checkAdjacentSquares(place1, place2)
// then queues up next move
void flood(){
    enqueue(mouse_x, mouse_y);
    //while queue isnt empty continue
    while (z != -1) {
        dequeue();
        checkAdjacentSquares(place1, place2);
        if (min >= maze[place1][place2] && min!=0 && min<50) { //only enqueue if smaller than min
            maze[place1][place2] = min + 1;
            if (place1 - 1 >= 0 && !hasKnownWall(place1, place2, 3) && check[place1][place2]) {
                enqueue(place1 - 1, place2);
            }
            if (place2 - 1 >= 0 && !hasKnownWall(place1, place2, 2) && check[place1][place2]) {
                enqueue(place1, place2 - 1);
            }
            if (place2 + 1 < MAZE_SIZE && !hasKnownWall(place1, place2, 0) && check[place1][place2]) { //only enqueue if it is known and has no wall in between
                enqueue(place1, place2 + 1);
            }
            // this was commented out lets put it back?
            if (place1 + 1 < MAZE_SIZE && !hasKnownWall(place1, place2, 1) && check[place1][place2]) {
                enqueue(place1 + 1, place2);
            }
        }
    }
}


// it is best practice to call flood() if there are any changes post mark wall
void checkWalls(int x, int y) {
    check[x][y] = 1;

    bool wallChanged = false;

    //lets use this for left and right now as it is easuer to read
    int right = (dir + 1) % 4;
    int left  = (dir + 3) % 4; // dir - 1 wrapped around

    // check front wall
    if (API_wallFront() && !hasKnownWall(x, y, dir)) {
        markWall(x, y, dir, true);
        wallChanged = true;
        printf("marking front wall !! \n");
    }

    // check right wall
    if (API_wallRight() && !hasKnownWall(x, y, right)) {
        markWall(x, y, right, true);
        wallChanged = true;
        printf("marking right wall !! \n");
    }

    // check left wall
    if (API_wallLeft() && !hasKnownWall(x, y, left)) {
        markWall(x, y, left, true);
        wallChanged = true;
        printf("marking left wall !! \n");
    }

    // trigger flood fill now if walls changed
    if (wallChanged) {
        flood();
    }
}




// init the wall array so that it is created at runtime rather than compile time, stack memory
// then as the mouse moves it will update its walls with information
void initWall() {
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            walls[x][y] = 0; // No internal walls
        }
    }
    for (int x = 0; x < MAZE_SIZE; x++) {
        walls[x][0] |= (1 << 2); // South walls
        walls[x][MAZE_SIZE - 1] |= (1 << 0); // North walls
    }
    for (int y = 0; y < MAZE_SIZE; y++) {
        walls[0][y] |= (1 << 3); // West walls
        walls[MAZE_SIZE - 1][y] |= (1 << 1); // East walls
    }
}


// new implementation of the mark walls function
void markWall(int x, int y, int dir, bool hasWall) {
  if (!hasWall) return; // lets skip if no wall

  //lets re use the dir since this *should* be the same as the one we are marking
  walls[x][y] |= (1 << dir);
  int nx = x, ny = y;
  // this snippet gives us opposite cordinate
  int opposite = (dir + 2) % 4;

  // mark walls
  switch (dir) {
    case 0: if (y < MAZE_SIZE - 1) ny++; break; // north
    case 1: if (x < MAZE_SIZE - 1) nx++; break; // east
    case 2: if (y > 0) ny--; break;             // south
    case 3: if (x > 0) nx--; break;             // west
  }
  // mark adjacent wall
  walls[nx][ny] |= (1 << opposite);
}

#endif /* INC_FLOODFILL_H_ */
