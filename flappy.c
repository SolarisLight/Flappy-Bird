// standard libraries (easily convertible to assembly, or no conversion needed)
#include <stdio.h>

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

/* takes in size of grid (sizeI and sizeJ), number of pipes to gen (pipeNum)
 pointer to game grid and pointer to pipe grid*/

void generatePipes(int sizeI, int sizeJ, int pipeNum,
 int pipePos[pipeNum][3], char grid[sizeI][sizeJ]){

}
int game(int sizeI, int sizeJ, char grid[sizeI][sizeJ]) {

int jumpSpace = 2;
int jumpGravity = 1;
int pipeNum = 2;

// store initial bird position
int birdPos[sizeI/2][0];;

/* generate pipes, and initial pipe positions. there can only be pipeNum of pipes
on the screen at once*/
/* 	 2d-array, i is pipe number, j[0] is x cord, j[1] is starting  y cord,
	 j[3] is ending y cord
	will be of size [pipeNum][3]
*/
// get pipePos declared
int pipePos[pipeNum][3];
// now fill it
generatePipes(sizeI, sizeJ, pipeNum, pipePos,  grid);
// now grid is a pointer - working yay
printf("%p", grid); // debug: prints the pointer
int collide = -1;

// after 10 iteritions, force while loop to end cause too lazy to change to 4 loop
int x = 0;

while (collide == -1) {


	// Get user input, they have 1 second
	int jump = -1;
	jump = checkKey();

	// user entered the enter key. move user up by jumpSpace
	if (jump == 1) {
		printf("%s", "jump");
	}
	// invalid input or user failed to respond, move bird down by gravitySpace
	else {
		printf("%s", "fall");
	}
	if (x == 10){
		collide = 1;
	}
	// Print out the new grid for the user
	x++;
}

return -1;
}

int main()
{
//  Create array of size 10 x 25

int sizeI = 10;
int sizeJ = 26;
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

