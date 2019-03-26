#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
//Array used for the gameBoard
char board[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

//List of Functions
int checkwin(int board_Size);
void gameBoard();
void tictactoe(int board_Size, struct node *list);
void append(struct node **list, int num);
void freeListMem(struct node ** list);
int undo(struct node **list, int gameCounter);
int redo(struct node **list, int gameCounter);
void writeList(struct node **list);
void readList(int board_Size);
#define MAX 10

//Create struct for doubly linked list
struct node
{
  int data;
  struct node * prev;
  struct node * next;
};



//Function to append to linked list
void append(struct node **list, int num)
{
  struct node *temp, *current = *list;
  if(*list == NULL)
  {
    *list = (struct node *) malloc(sizeof(struct node));
    memset(*list, 0, sizeof(struct node));
    (*list) -> prev = NULL;
    (*list) -> data = num;
    (*list) -> next = NULL;
  }
  else
  {
    while(current -> next != NULL)
      current = current -> next;

    temp = (struct node *) malloc(sizeof(struct node));
    temp -> data = num;
    temp -> next = NULL;
    temp -> prev = current;
    current -> next = temp;
  }
}

//Function to free each node from the linked list.
void freeListMem(struct node ** list)
{
  struct node *currentNode = *list;
  struct node *nextNode;

  while ((currentNode) != NULL)
  {
    nextNode = currentNode ->next;
    free (currentNode);
    currentNode = nextNode;
  }
  *list = NULL;
}


//Main method
int main()
{
  int board_Size = 10;
  //Create linked list
  struct node * list;
  list = NULL;
  gameBoard();
  tictactoe(board_Size, list);
  checkwin(board_Size);
}

//Method to check if the game has been won
int checkwin(int board_Size)
{
  if (board[1] == board[2] && board[2] == board[3])
       return 1;

   else if (board[4] == board[5] && board[5] == board[6])
       return 1;

   else if (board[7] == board[8] && board[8] == board[9])
       return 1;

   else if (board[1] == board[4] && board[4] == board[7])
       return 1;

   else if (board[2] == board[5] && board[5] == board[8])
       return 1;

   else if (board[3] == board[6] && board[6] == board[9])
       return 1;

   else if (board[1] == board[5] && board[5] == board[9])
       return 1;

   else if (board[3] == board[5] && board[5] == board[7])
       return 1;

    else for(int i = 0; i < board_Size; i++)
    {
      int drawCounter = 0;
      if(board[i] = "X" || "O")
      {
        drawCounter++;
      }
      if(drawCounter = 9)
      {
        return 0;
      }

    }
    return -1;
}

//The game function
void tictactoe(int board_Size, struct node *list)
{
    char input[16];
    int player = 0, j = -1, choice;
    char mark;
    int gameCounter = -1;
    while (gameCounter != 9)
    {
      //Determines which players turn it is
       if(gameCounter % 2 == 0)
       {
         player = 1;
         mark = 'X';
         printf("Its player 1s turn!\n");
       }
       else
       {
         player = 2;
         mark = 'O';
         printf("Its player 2s turn!\n");
       }

        //Checks if the user has entered any special commands
        scanf("%s", input);

        if(strcmp(input, "undo") == 0)
        {
          gameCounter = undo(&list, gameCounter);
        }

      else if(strcmp(input, "redo") == 0)
        {
          gameCounter = redo(&list, gameCounter);
        }
      else if(strcmp(input, "replay") == 0)
      {
        readList(board_Size);
        exit(0);
      }
        else
        {
          //Appends users entry to the linked list if its valid
          sscanf(input, "%d", &choice);
          if(choice <= 9 && choice > 0)
          {
            append(&list, choice);
          }
          else
          {
            printf("Invalid: please enter a number between 1 and 9\n");
          }

        //Changes the board to represent the users input
        for(int i=0; i < board_Size; i++){
          int boardPos = board[i] - '0';
          if(choice == boardPos && boardPos != mark)
          {
            board[i] = mark;
            gameBoard();
          }
        //Checks to see if the game has been won
         j = checkwin(board_Size);
        //Changes the turn to the next player
         player++;
      }

        if(j == -1)
        {
          gameBoard();
        }

        //Runs if the game has been won, saves the game data and frees the ll memory
        if(j == 1){
          printf("Congratulations %c wins the game\n", mark);
          writeList(&list);
          freeListMem(&list);
          printf("Do you want to play again? (y/n) \n");
          scanf("%s", input);

          if(strcmp(input, "y") == 0)
          {
            for(int r = 0; r < board_Size; r++)
            {
              board[r] = r + '0';
            }
          gameCounter = -1;
          gameBoard();
        }
        else if(strcmp(input, "n") == 0)
        {
          exit(0);
        }
      }
      gameCounter++;
  }


  }

}

//Function to allow users to undo moves
int undo(struct node **list, int gameCounter)
{
  struct node *temp = *list;
  for (int i = 0; i < gameCounter; i++)
  {
    if(temp -> next != NULL)
    {
      temp = temp -> next;
    }
  }
  int value, i;
  value = temp -> data;
  board[temp->data] = temp -> data + '0';
  gameBoard();
  if(temp -> prev == NULL)
  {
    gameCounter = gameCounter + 1;
  }
  gameCounter--;
  return gameCounter;
}

//Function to allow users to redo undid moves
int redo(struct node **list, int gameCounter)
{
  struct node *temp = *list;
  for (int i = 0; i < gameCounter; i++)
  {
    if(temp -> next != NULL)
    {
      temp = temp -> next;
    }
  }
  int value, i;
  temp = temp -> next;
  value = temp -> data;
  if(gameCounter % 2 == 0)
  {
  board[temp->data] = 'X';
  }
  else
  {
    board[temp->data] = 'O';
  }
  gameBoard();
  gameCounter++;
  return gameCounter;
}

//Function to write the contents of the linked list to file
void writeList(struct node **list)
{
  FILE *pFile;
  //Sets the file to the appropriate file.
  pFile = fopen("Tic-Tac-Toe.txt", "w+");

  if(pFile != NULL)
  {
    struct node *temp = *list;
    //Appends to the file while the next node is not null
    while(temp->next != NULL)
    {
      fseek(pFile, 0, SEEK_END);
      fprintf(pFile, "%d", temp->data);
      temp = temp->next;
    }
    fprintf(pFile, "%d\n", temp->data);
    fclose(pFile);
    pFile = NULL;
  }
  else
  {
    printf("FILE OPEN ERROR\n");
  }

}
//Function to read the file to replay the last game played
void readList(int board_Size)
{
  FILE *myFile;
  myFile = fopen("Tic-Tac-Toe.txt", "r");
  int value = 0;
  char testArray[9];
  char line[9];
  int counter = 0;

  if (fgets(testArray, 9, myFile) != NULL)
    {

    }
    fclose(myFile);
  size_t len = strlen(testArray);

  for(int i = 0; i < len - 1; i++)
  {
    if(testArray[i] == testArray[1] || testArray[i] == testArray[3] || testArray[i] == testArray[5] || testArray[i] == testArray[7] || testArray[i] == testArray[9] )
    {
      value = testArray[i] - '0';
      printf("Player 1 placed an X in position: %1c\n", testArray[i]);
      board[value] = 'X';
    }
    else
    {
      value = testArray[i] - '0';
      printf("Player 2 placed an O in position: %1c\n", testArray[i]);
      board[value] = 'O';
    }
    Sleep(2000);
    gameBoard();
  }
}

//Displays the tic-tac-toe board in the correct style and with the correct values
void gameBoard()
{
  printf("\n\n Tic Tac Toe \n\n");

  printf("Player 1 (X) - Player 2 (O)\n\n\n");

  printf("Choose a cell numbered from 1 to 9 as shown below and play \n\n");
   printf("     |     |     \n");
   printf("  %c  |  %c  |  %c \n", board[1], board[2], board[3]);

   printf("_____|_____|_____\n");
   printf("     |     |     \n");

   printf("  %c  |  %c  |  %c \n", board[4], board[5], board[6]);

   printf("_____|_____|_____\n");
   printf("     |     |     \n");

   printf("  %c  |  %c  |  %c \n", board[7], board[8], board[9]);

   printf("     |     |     \n\n");
}
