#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

#define SCR_WIDTH gfx_screenWidth()
#define SCR_HEIGHT gfx_screenHeight()

#define NUMBER_OF_POLES 3
#define DISTANCE SCR_WIDTH / (NUMBER_OF_POLES + 1)
#define FLOOR SCR_HEIGHT - 20

#define WIDTH_OF_POLE 10
#define HEIGHT_OF_POLE 200
#define MIDDLE_OF_POLE DISTANCE + (WIDTH_OF_POLE / 2)

#define NUMBER_OF_BLOCKS 5
#define HEIGHT_OF_BLOCK 15

#define WIDTH_OF_BLOCK 30
#define HALF_WIDTH_OF_FIRST_BLOCK 30     
#define HALF_WIDTH_OF_BLOCK WIDTH_OF_BLOCK / 2
#define MAX_HEIGHT_OF_BLOCKS HEIGHT_OF_BLOCK* NUMBER_OF_BLOCKS

#define ANIMATION_HEIGHT 300

void Arrays(int array[], int X[], int Y[])
{

	for (int i = NUMBER_OF_BLOCKS; i > 0; i--) {
		array[i] = i;
		X[i] = MIDDLE_OF_POLE;
		Y[i] = FLOOR - ((NUMBER_OF_BLOCKS - i) * HEIGHT_OF_BLOCK);
	}
}

void Draw(int X_ARRAY[], int Y_ARRAY[])
{
	gfx_filledRect(0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, BLACK);
	for (int i = 1; i <= NUMBER_OF_POLES; i++) {
		int x1 = DISTANCE * i;
		int y1 = SCR_HEIGHT - HEIGHT_OF_POLE;
		int x2 = x1 + WIDTH_OF_POLE;
		int y2 = SCR_HEIGHT;
		gfx_filledRect(x1, y1, x2, y2, GREEN);
	}
	gfx_filledRect(0, FLOOR, SCR_WIDTH, SCR_HEIGHT, YELLOW);
	for (int i = 1; i <= NUMBER_OF_BLOCKS; i++) {
		int x1 = X_ARRAY[i] - HALF_WIDTH_OF_BLOCK * i;
		int y1 = Y_ARRAY[i] - HEIGHT_OF_BLOCK - 1;
		int x2 = X_ARRAY[i] + HALF_WIDTH_OF_BLOCK * i;
		int y2 = Y_ARRAY[i] - 1;
		gfx_filledRect(x1, y1, x2, y2, RED);
	}
}
void FillEmptyArray(int array[NUMBER_OF_POLES][NUMBER_OF_BLOCKS])
{
	for (int i = 0; i < NUMBER_OF_POLES; i++) {
		for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
			array[i][j] = 0;
		}
	}
}
void Start(int array[NUMBER_OF_POLES][NUMBER_OF_BLOCKS])
{
	int k = NUMBER_OF_BLOCKS;
	for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
		array[0][j] = k;
		k = k - 1;
	}
}
void PrintArray(int array[NUMBER_OF_POLES][NUMBER_OF_BLOCKS])
{
	for (int i = 0; i < NUMBER_OF_POLES; i++) {
		for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
			printf("%d ", array[i][j]);
		}
		printf("\n ");
	}
	printf("\n ");
	printf("\n ");
	printf("\n ");
}
void ChangeArray(int array[NUMBER_OF_POLES][NUMBER_OF_BLOCKS], int NrOfPoleFrom,
				 int NrOfPoleTo, int Xar[], int Yar[])
{
	if (NrOfPoleTo > NUMBER_OF_POLES) {
		printf("\n Wrong value! \n");
	}
	else if (NrOfPoleTo == NrOfPoleFrom) {
		printf("\n Wrong value! \n");
	}
	else {
		NrOfPoleFrom = NrOfPoleFrom - 1;
		NrOfPoleTo = NrOfPoleTo - 1;

		int X = 0;
		int Y = 0;
		int licznik = 0;
		for (int j = NUMBER_OF_BLOCKS - 1; j >= 0; j--) {
			if (array[NrOfPoleFrom][j] != 0) {
				X = array[NrOfPoleFrom][j];
				licznik = j;
				break;
			}
		}
		for (int j = 0; j < NUMBER_OF_BLOCKS; j++) {
			if (array[NrOfPoleTo][j] == 0) {
				if (j > 0 && array[NrOfPoleTo][j - 1] < X) {
					printf("\n Wrong value! \n");
				}
				else {
					array[NrOfPoleTo][j] = X;
					array[NrOfPoleFrom][licznik] = 0;
					Y = j;
					int difference = NrOfPoleTo - NrOfPoleFrom;
					int odleglosc = difference * DISTANCE;
					for (int i = 1; i <= NUMBER_OF_BLOCKS; i++) {
						if (X == i) {
							Xar[i] += odleglosc;
							Yar[i] = FLOOR - Y * HEIGHT_OF_BLOCK;
						}
					}
					break;
				}
			}
		}
	}
}

void CheckWin(int array[NUMBER_OF_POLES][NUMBER_OF_BLOCKS])
{
	if (array[NUMBER_OF_POLES - 1][NUMBER_OF_BLOCKS - 1] != 0) {
		gfx_textout(SCR_WIDTH / 2, SCR_HEIGHT / 2, "You won", GREEN);
	}
}
void SaveValues(int X[], int Y[],int checkX[],int checkY[]){
	for (int i = 1; i <= NUMBER_OF_BLOCKS; i++) {
			checkX[i] = X[i];
		}
		for (int i = 1; i <= NUMBER_OF_BLOCKS; i++) {
			checkY[i] = Y[i];
		}
}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	int array[NUMBER_OF_BLOCKS + 1];
	int X[NUMBER_OF_BLOCKS + 1];
	int Y[NUMBER_OF_BLOCKS + 1];
	int Placement[NUMBER_OF_POLES][NUMBER_OF_BLOCKS];
	int checkX[NUMBER_OF_BLOCKS + 1];
	int checkY[NUMBER_OF_BLOCKS + 1];
	
	FillEmptyArray(Placement);

	Arrays(array, X, Y);
	
	Start(Placement);

	while (1) {
		Draw(X, Y);
		CheckWin(Placement);
		gfx_updateScreen();
		PrintArray(Placement);
		int val1 = gfx_getkey() - 48;
		int val2 = gfx_getkey() - 48;

		SaveValues(X, Y, checkX, checkY);

		ChangeArray(Placement, val1, val2, X, Y);

		for (int i = 1; i <= NUMBER_OF_BLOCKS; i++) {
			if (checkX[i] != X[i]) {
				int celX = X[i];
				int celY = Y[i];
				if (checkX[i] < X[i]) {
					X[i] = checkX[i];
					Y[i] = checkY[i];
					while (X[i] != celX || Y[i] != celY) {
						if (Y[i] > ANIMATION_HEIGHT && X[i] == checkX[i]) {
							Y[i] -= 1;
						}
						if (Y[i] == ANIMATION_HEIGHT && X[i] != celX) {
							X[i] += 1;
						}
						if (X[i] == celX && Y[i] != celY) {
							Y[i] += 1;
						}

						Draw(X, Y);
						gfx_updateScreen();
					}
				}
				else {
					X[i] = checkX[i];
					Y[i] = checkY[i];
					while (X[i] != celX || Y[i] != celY) {
						if (Y[i] > ANIMATION_HEIGHT && X[i] == checkX[i]) {
							Y[i] -= 1;
						}
						if (Y[i] == ANIMATION_HEIGHT && X[i] != celX) {
							X[i] -= 1;
						}
						if (X[i] == celX && Y[i] != celY) {
							Y[i] += 1;
						}

						Draw(X, Y);
						gfx_updateScreen();
					}
				}
			}
		}

		SDL_Delay(10);
		if (gfx_isKeyDown(SDLK_ESCAPE)) {
			return 0;
		}
	}

	return 0;
}
