#include <stdlib.h>
#include <stdio.h>

//Declare constants here

#define ARRAY_LENGTH 100
#define BUFFER_SIZE 10

void getLine(FILE *filePointer, int lineNum)
{
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, (FILE*)filePointer);
    printf("Line %d: %s\n", lineNum, buffer);
}

//This code is "Space efficient since we are only storing the pointer, but not time efficient"

int main()
{
    FILE **pointerList = malloc(ARRAY_LENGTH * sizeof(FILE*));
    
   char buff[BUFFER_SIZE];
   
   char temp_buff[BUFFER_SIZE];
   
   int count = 0;
   
   int i;
   
   for(i = 0; i < ARRAY_LENGTH; i++)
   {
      pointerList[i] = fopen("input.txt", "r");
      int j;
      if(count)
      {
          for(j = 0; j < count; j++)
          {
            fgets(temp_buff, BUFFER_SIZE, pointerList[i]);  
          }
      }
      count++;
   }
   
   //takes the pointer and the Line number and does something: "Right now it is just print the line"
    getLine(pointerList[5], 6);
    
   fclose(pointerList[0]);

    return 0;
}
