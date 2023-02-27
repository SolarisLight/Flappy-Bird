// standard libraries (easily convertible to assembly, or no conversion needed)
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// these two are for system calls,
// which will be able to be translated cleanly to assembly
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>


// gives user 1 second to hit enter to jump the bird, else the bird goes down
// key to be pressed is just the enter key
int checkKey() {
    char            name[20] = {0}; // in case of single character input
    fd_set          input_set;
    struct timeval  timeout;
    int             ready_for_reading = 0;
    int             read_bytes = 0;
    int entry = -10;
    /* Empty the FD Set */
    FD_ZERO(&input_set );
    /* Listen to the input descriptor */
    FD_SET(STDIN_FILENO, &input_set);

    /* Waiting for some seconds */
    timeout.tv_sec = 1;    // WAIT seconds
    timeout.tv_usec = 0;    // 0 milliseconds


    /* Listening for input stream for any activity */
    ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);
    /* Here, first parameter is number of FDs in the set, 
     * second is our FD set for reading,
     * third is the FD set in which any write activity needs to updated,
     * which is not required in this case. 
     * Fourth is timeout
     */

    if (ready_for_reading == -1) {
        /* Some error has occured in input */
        return -1;
    }

    if (ready_for_reading) {
        read_bytes = read(0, name, 19);
        if(name[read_bytes-1]=='\n'){
        --read_bytes;
        name[read_bytes]='\0';
        }
        if(read_bytes==0){ // hit enter
            entry = 1;
        } else { // hit other key, then enter
            entry = -1;
        }
    } else { // timeout
        entry = -1;
    }
	return entry;
}

/*	Generates a new pipe for when a pipe goes out of bunds. generates it towards the right side of screen
	NOT TESTED, but DOES COMPILE! Cause I didn't implement the moving of pipes yet
*/
void createNewPipe(int sizeI, int sizeJ, int pipeNum, int pipeNumToRegen,
                         int pipePos[pipeNum][3], char grid[sizeI][sizeJ]){
        srand(time(NULL));

	int i = pipeNumToRegen;
        int minPos = sizeJ-sizeJ/4;
        int maxPos = sizeJ-1;
        int minLen = sizeI/3;
        int maxLen = sizeI/2-2;

        // so we have pipeNUm of pipes at any given time so
                // get a random position, length
                int pos = (rand() % (maxPos - minPos +1)) + minPos;
                int len = (rand() % (maxLen - minLen +1)) + minLen;

                // start to populate pipePos
                pipePos[i][0] = pos;
                pipePos[i][1] = len;
                pipePos[i][2] = 1; // set initial pipe to be from bottom
                int initialPos = sizeI-1;
                // every other pipe will be on opposite sides. starting with pipe index 1

		// if pipenum is 1, then its a t
                if (pipePos[i][2] == -2) {
                        pipePos[i][2] = -2;     // Top side
                        initialPos = 0;

                        // populate grid
                        for (int j = 0; j < len; j++) {
                                grid[j][pos] = '|';
                        }

                } else { // bottom side, go from bottom up
                        // populate grid

                         for (int j = sizeI-1; j > sizeI-len; j--) {
                                grid[j][pos] =  '|';
                        }

                }
                //fflush(stdout);



}

/* Attempts to move the pipes, returns 1 on sucess, or -1 upon bird collision.
	Also handles pipes going out of bounds, and will call createNewPipes if too many go out of bounds.

*/


     /*       2d-array, i is pipe number, j[0] is pos (horizontal), j[1] is len, j[2] top(-1) or bottom (1)
                will be of size [pipeNum][3]
        */

int movePipes(int sizeI, int sizeJ, int pipeNum,
                         int pipePos[pipeNum][3], char grid[sizeI][sizeJ]){

	// Defualt case - success, intially set result to -1
	int result = 1;

	int pipesToRemove = 0; // Default is that no pipes go out of bounds
	// Now loop through each pipe, attempt to move it to the left
	for (int i = 0; i < pipeNum; i++) {

		// out of bounds check
		if (pipePos[i][0] - 1 == -1) { // we gotta regen it cause it will be out of bounds
			createNewPipe(sizeI, sizeJ, pipeNum, i, pipePos, grid);

		} else { // we good, it can be moved, now enter loop to move entirety of pipe
			// no need to check for collision with the new pipe, as it is on opposite side

			// get len and pos
			int pos = pipePos[i][0];
			int len = pipePos[i][1];
			// every other pipe will be on opposite sides. index 0 is on bottom, 1 top etc...
			if (i % 2 == 0) {		// Shift pipe (top) over to left
				 for (int j = 1; j < len; j++) {

					// check for collide with bird
                                	if (grid[j][pos-1] == 'a') {
					return -1;
					}

					grid[j][pos] = ' ';
					grid[j][pos-1] = '|';
                       		 }


			}

			else { 				// Shift pipe (bottom) over to left
				for (int j = sizeI-1; j > sizeI-len; j--) {
					// check for collide with bird
					if (grid[j][pos-1] == 'a') {
						return -1;
					}

					// Shift pipe over
					grid[j][pos] = ' ';
					grid[j][pos-1] = '|';


				}

			}
			// Update pipePos
			pipePos[i][0] -=1;

		}

	}


	// sucess
	return result;
}


/* takes in size of grid (sizeI and sizeJ), number of pipes to gen (pipeNum)
 pointer to game grid and pointer to pipe grid*/
void generatePipes(int sizeI, int sizeJ, int pipeNum,
			 int pipePos[pipeNum][3], char grid[sizeI][sizeJ]){

	srand(time(NULL));
	int minPos = sizeJ/5;
	int maxPos = sizeJ-1;
	int minLen = sizeI/3;
	int maxLen = sizeI/2-2;

	// so we have pipeNUm of pipes at any given time so
	for (int i = 0; i < pipeNum; i++){
		// get a random position, length
		int pos = (rand() % (maxPos - minPos +1)) + minPos;
		int len = (rand() % (maxLen - minLen +1)) + minLen;

		// start to populate pipePos
		pipePos[i][0] = pos;
		pipePos[i][1] = len;
		pipePos[i][2] = 1; // set initial pipe to be from bottom
		int initialPos = sizeI-1;
		// every other pipe will be on opposite sides. starting with pipe index 1
		if (i % 2 == 0) {
			pipePos[i][2] = -2;	// Top side
			initialPos = 0;

			// populate grid
			for (int j = 0; j < len; j++) {
				grid[j][pos] = '|';
			}

		} else { // bottom side, go from bottom up
			// populate grid

			 for (int j = sizeI-1; j > sizeI-len; j--) {
				grid[j][pos] =  '|';
                        }

		}
		//fflush(stdout);

	}

}


int game(int sizeI, int sizeJ, char grid[sizeI][sizeJ]) {

	int score = 0;
	int numLoops = 10;
	int pipeNum = 10;

	// store initial bird position
	int birdPos = sizeI/2;

	/* generate pipes, and initial pipe positions. there can only be pipeNum of pipes
	on the screen at once*/
	/* 	 2d-array, i is pipe number, j[0] is pos (horizontal), j[1] is len, j[2] top(-1) or bottom (1)
		will be of size [pipeNum][3]
	*/
	// get pipePos declared
	int pipePos[pipeNum][3];
	// now fill it
	generatePipes(sizeI, sizeJ, pipeNum, pipePos, grid);
	// now grid is a pointer - working yay
	// printf("%p", grid); // debug: prints the pointer
	int collide = -1;

	// after 10 iteritions, force while loop to end cause too lazy to change to a for loop
	int x = 0;
	while (collide == -1) {

		// Get user input, they have 1 second
		int jump = -1;
		jump = checkKey();

		// Remove old bird Pos
		grid[birdPos][0] = ' ';

		// user entered the enter key. move user up by jumpSpace
		if (jump == 1) {
			fflush(stdout); // C hates me, force flush the stream out so it prints now, not after loop

			// Update bird pos in grid
			birdPos -= sizeI/2/5;
		}
		// invalid input or user failed to respond, move bird down by gravitySpace
		else {

			// Update bidPos
			birdPos += sizeI/8;
			fflush(stdout); // same as above comment
		}

		// Put in new birdPos
		grid[birdPos][0] = 'a';
		// Now print the grid
		for (int i = 0; i < sizeI; i++) {
        		for (int j = 0; j < sizeJ; j++) {
       				 printf("%c", grid[i][j]);
        		}
      		  printf("%c", '\n');
		}



		if (x == numLoops){
			collide = 1;
		}

		x++;

		// now attempt to  move all the pipes to the left
		int result = movePipes(sizeI, sizeJ, pipeNum, pipePos, grid);

		// If result is 1, success, if -1, exit the game because collision with bird
		if (result == -1) {
			return score;
		}


	}

	return score;
}

int main() {
	//  Create array of size 40 x 60

	int sizeI = 20;
	int sizeJ = 40;
	char grid[sizeI][sizeJ];

	// Populate array with empty space
	for (int i = 0; i < sizeI; i++) {
		for (int j = 0; j < sizeJ; j++) {
			grid[i][j] = ' ';
		}
	}

	// Create the bird (bird will be the character 'a'
	grid[sizeI/2][0] =  'a';

	// Create the ground (ground will be character '_'
	for (int j = 0; j < sizeJ; j++) {
		grid[sizeI-1][j] = '_';
	}


	/*	 Debug stuff to ensure I remember how to print an array of known
		size wth the starting pointer*/
	/*
	char (*ptrGrid)[sizeI][sizeJ];
	ptrGrid = &grid;

	for (int i = 0; i < sizeI; i++) {
       		 for (int j = 0; j < sizeJ; j++) {
      		  printf("%c", (*ptrGrid)[i][j]);

      		  }
      		  printf("%c", '\n');
	}
	*/

	// Call the game function, paramater is pointer to array, size of array
	int score = game(sizeI, sizeJ, grid);

	// working = changes made to grid in game are done right, is updated here

	printf("%s %i, %s", "\nGame over! Your score is: ", score, ". Come back soon!\n");
	return 0;
}

