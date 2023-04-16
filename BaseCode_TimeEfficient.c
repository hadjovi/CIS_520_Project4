#include <stdlib.h>
#include <stdio.h>

//Declare constants here

#define ARRAY_LENGTH 100
#define BUFFER_SIZE 10000

void getLine(char *line, int lineNum)
{
    printf("Line %d: %s\n", lineNum, line);
}


int main()
{
    FILE *filePointer;
    
    filePointer = fopen("input.txt", "r");
    
    char buffList[ARRAY_LENGTH][BUFFER_SIZE];
    
    int i;
   
   for(i = 0; i < ARRAY_LENGTH; i++)
   {
      fgets(buffList[i], BUFFER_SIZE, filePointer);  
   }
   
    getLine(buffList[9], 10);
    
   fclose(filePointer);

    return 0;
}