#include<stdio.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define noOfPrizes 13
#define noOfRounds 3

#define ESC "\e["
#define YELLOW "33m"

const int thePrizes[noOfPrizes] = {1, 2, 5, 10, 20, 50, 80, 100, 200, 500, 1000, 2000, 5000};
const int rounds[noOfRounds] = { 5, 3, 3 };

typedef struct {
  int prize;
  bool opened;
} Box;

int doGame();
char* getString(char* msg, char* buff, int len);
void revealAllBoxes(Box *boxes, int noBoxes);
void showAvailablePrizes(Box *boxes, int noBoxes);
void showClosedBoxes(Box *boxes, int noBoxes, int myBox);
float average(int *values, int size);
int getOffer(Box *boxes, int noBoxes);

bool colourEnabled = false;
void setColour(char* col) {
  if (colourEnabled) printf("%s%s", ESC, col);
}
char* getString(char* msg, char* buff, int len) {
  printf("%s", msg);
  scanf(" %s", buff);
  return buff;
}

int getNum() {
  int myBox;
  while (!scanf("%d%*c", &myBox) || (myBox < 0 || myBox > noOfPrizes)) {
    printf("Invalid entry, please chose a valid box \n");
    while(getchar() != '\n');
  }
  if (myBox == 0) {
    printf("Ending game, thanks for playing... 73\n");
    exit(0); // terminate
  }
  return myBox;
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);
  sleep(1);
  return doGame();
}

int doGame() {
  setColour(YELLOW);
  printf("---------------------------\r\n");
  printf("Welcome to deal or no deal!\r\n");
  printf("---------------------------\r\n\n");
  printf("\e[37m");

  char name[32] = {};
  printf("Please enter your name: ");
  scanf("%[^\n]s", (char*)&name);
  printf("Hello %s, welcome to the game, good luck! (enter 0 to exit)\r\n", name);

  Box boxes[noOfPrizes];
  for(int i = 0; i < noOfPrizes; i++) {
    boxes[i].prize = 0;
    boxes[i].opened = false;
  }

  printf("There are %d boxes with unique prizes in each\r\n", noOfPrizes);
  printf("Available prizes are:\r\n\n");
  for (int i = 0; i < noOfPrizes; i++) {
    printf("$%d ", thePrizes[i]);
  }
  printf("\n\r\n\r");
  
  printf("Please pick a valid box by number (+ enter): ");
  int myBox = getNum();
  printf("You have chosen box %d\n\r", myBox);
 
  srand(time(NULL));   // Initialization, should only be called once.
//  int r = rand() % noOfPrizes;
//  printf("%d\r\n", r);

  for(int i = 0; i < noOfPrizes; i++) {
    int bn = rand() % noOfPrizes;
    while(boxes[bn].prize > 0) {
      bn = rand() % noOfPrizes;
    }
    boxes[bn].prize = thePrizes[i];
  }

  showAvailablePrizes(boxes, noOfPrizes);
  
  for (int rn = 0; rn < noOfRounds; rn++) {
    int roundSize = rounds[rn];
    printf("\n\rRound %d, pick %d boxes\n\r", rn+1, roundSize);
  
    for(int i = 0; i < roundSize; i++) {
      showClosedBoxes(boxes, noOfPrizes, myBox);
      printf("\n\r%d - pick an available box number (+ enter): ", i+1);
      int bn = getNum();
      while( (bn < 1) || (bn > noOfPrizes) || (boxes[bn-1].opened == true) || bn == myBox) {
        printf("Box %d is not available, pick another: ", bn);
        bn = getNum();
      }
      printf("Picked %d, opening....\n\r", bn);
      sleep(3);
      boxes[bn-1].opened = true;
      printf("\a");
      printf("Box %d prize is ***** $%d ***** \n\r", bn, boxes[bn-1].prize);
    }

    showAvailablePrizes(boxes, noOfPrizes);

    printf("\n\rIncomming call from the banker! ...\n\r");
    sleep(2);
    printf("  ring ring...\n\r");
    sleep(2);
    printf("  ring ring...\n\r");
    int offer = getOffer(boxes, noOfPrizes);
    printf("The Banker has made you an offer of ");
    sleep(2);
    printf("***** $%d *****, accept y/n \n\r", offer);
    char yn;
    scanf(" %c", &yn);
    while( yn != 'y' && yn != 'n') {
      printf("Invalid input, please type y or n (+ enter)");
      scanf(" %c", &yn);
    }
    if (yn == 'y') {
      printf("Well played %s, you won $%d \n\r", name, offer);
      printf("Please come back and play again soon.\n\r");
      exit(0);
    }
    printf("\n\rOkay, good luck... \n\r");
  }
  
  showAvailablePrizes(boxes, noOfPrizes);
  showClosedBoxes(boxes, noOfPrizes, myBox);

  printf("Your box number is %d\n\r", myBox);

  printf("\n\rWould you like to swap your box with the remaining box?\n\r");
  char swapBox;
  scanf(" %c", &swapBox);
  while( swapBox != 'y' && swapBox != 'n') {
    printf("Invalid input, please type y or n + enter");
    scanf(" %c", &swapBox);
  }
  if (swapBox == 'y') {
    printf("You chose to swap your box with the remaining box...\n\r");
    int remainingBox = -1;
    for (int i = 0; i < noOfPrizes; i++ ) {
      if ((i+1 != myBox) && (boxes[i].opened == false)) {
        remainingBox = i;
      }
    }
    printf("Opening box %d. You win $%d\n\r\r", remainingBox+1, boxes[remainingBox].prize);
  } else {
    printf("You chose to keep your original box...\n\r");
    printf("Opening box %d. You win $%d\n\r\r", myBox, boxes[myBox-1].prize);
  }

  printf("\n\r");
  printf("Congratulations, %s please play again soon!\r\n", name);
  return 0;
}

void revealAllBoxes(Box *boxes, int noBoxes) {
  printf("\n\rAll boxes are:\r\n");
  for (int i = 0; i < noOfPrizes; i++) {
    printf("Box no %d is prize %d, opened %s\n\r", i+1, boxes[i].prize, boxes[i].opened ? "true" : "false");
  }
  return;
}

void showAvailablePrizes(Box *boxes, int noBoxes) {
  printf("\n\rAvailable Prizes:\n\r");
  for (int pn = 0; pn < noOfPrizes; pn++) {
    for (int bn = 0; bn < noBoxes; bn++) {
      if (boxes[bn].opened == false && boxes[bn].prize == thePrizes[pn]) {
        printf(" $%d ", boxes[bn].prize);
      }
    }
  }
  printf("\n\r");
  return;
}

void showClosedBoxes(Box *boxes, int noBoxes, int myBox) {
  printf("\n\rAvailable closed boxes are:\n\r");
  for (int i = 0; i < noBoxes; i++) {
    if (boxes[i].opened == false && (i+1 != myBox)) {
      printf(" %d ", i+1);
    }
  }
  printf("\n\r");
  return;
}


float average(int *values, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum = sum + values[i];
//    printf("averaging %d, %d, %d\n\r", i, values[i], sum);
  }
  return (float)sum / size;
}

int getOffer(Box *boxes, const int noBoxes) {
  int size = 0;
  int prizes[noBoxes];
  for (int i = 0; i < noBoxes; i++) {
    if (boxes[i].opened == false) {
      prizes[size] = boxes[i].prize;
      size++;
    }
  }
  int avg = (int)average(prizes, size);
  return avg;
}
