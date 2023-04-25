#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//--------------- Declare constants here --------------//
#define ARRAY_LENGTH 100
#define BUFFER_SIZE 5000

//--------------- Declare Global Variable here --------------//
 
int LineCounter = 0;

//--------------- Declare Functions here --------------//
void getLine(char *line, int lineNum)
{
    printf("Line %d: %s\n", lineNum, line);
}


int main()
{
    FILE *filePointer;
    
    filePointer = fopen("../wiki_10.txt", "r");
    
    //Check if file opened successfully
    if (filePointer == NULL) 
    {
        printf("Could not open file\n");
        return 1;
    }

    //Create a temporary buffer to get the data
    char buffList[ARRAY_LENGTH][BUFFER_SIZE];
    
    int i;
   
    printf("About to start reading\n");

    //read file until end of stream 
    
    i = 0;
    while(fgets(buffList[i], BUFFER_SIZE, filePointer))
    {
        printf("\n\n");
        printf("Line %d: %s", ++LineCounter, buffList[i]);
        i++;
    }

    printf("\n Number of Line is %d", LineCounter);

    //copy data to a Wellsize array and destroy old buff
    char fileBuff[LineCounter][BUFFER_SIZE];
    memcpy(fileBuff, buffList, LineCounter * BUFFER_SIZE);

    // for(i = 0; i < ARRAY_LENGTH; i++)
    // {
    //     fgets(buffList[i], BUFFER_SIZE, filePointer);
    //     printf("Line %d: %s", i, buffList[i]);  
    // }
   
//    for(i = 0; i < ARRAY_LENGTH; i++)
//    {
//       getLine(buffList[i], i+1);  
//    }
    
    
   fclose(filePointer);

    return 0;
}