#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//--------------- Declare constants here --------------//
#define ARRAY_LENGTH 100
#define BUFFER_SIZE 2005

//--------------- Declare Global Variable here --------------//
 
int LineCounter = 0;

char **fileBuff;

int *maxValues;

//--------------- Declare Functions Prototype here --------------//
// void getMax(int, int, char (*)[BUFFER_SIZE] );

// void getLine(char *line, int lineNum);


//--------------- Declare Functions here --------------//
void getLine(char *line, int lineNum)
{
    printf("Line %d: %s\n", lineNum, line);
}

void getMax(int start, int stop)
{
    int len;
    int max;
    int cur;
    for(int i = start-1; i < stop; i++)
    {
        len = strlen(fileBuff[i]);
        max = 0;
        for(int j = 0; j < len-1; j++)
        {
            cur = (int) fileBuff[i][j];
            if(cur > max && cur < 123)
            {
                max = cur;
            }
        }
        maxValues[i] = max;
        printf("Line %d Max: %d\n", i+1, max);
        //printf("Line %d: %s", i+1, fileBuff[i]);  
    }  
}

int main()
{
    FILE *filePointer;
    
    filePointer = fopen("../wiki_100.txt", "r");
    
    //Check if file opened successfully
    if (filePointer == NULL) 
    {
        printf("Could not open file\n");
        return 1;
    }

    //Create a temporary buffer to get the data
    char buffLine[BUFFER_SIZE];
       
    printf("About to start reading\n");

    int len;

    //read file until end of stream -- get the Counter
    while(fgets(buffLine, BUFFER_SIZE, filePointer))
    {
        //update Counter
        LineCounter++;
        len = strlen(buffLine);
        // printf("\n\n");
        // printf("Line Lenght: %d\n", len);
        // printf("Line %d: %s", LineCounter, buffLine);
    }
    rewind(filePointer);

    printf("\n Number of Line is %d\n", LineCounter);

    //copy data to a file buffer

    //char fileBuff[LineCounter][BUFFER_SIZE];
    
    fileBuff = malloc(LineCounter * sizeof(char *));       

    if (fileBuff == NULL) 
    {
        printf("Could not allocate memory for array\n");
        return 1;
    }

    for(int i = 0; i < LineCounter; i++)
    {
        fileBuff[i] = (char *) malloc(BUFFER_SIZE * sizeof(char));

        if (fileBuff[i] == NULL) 
        {
            printf("Could not allocate memory for array[%d]", i);
            return 1;
        } 
    } 

    // Read file line by line and store each line in the array
    for (int i = 0; i < LineCounter; i++) 
    {
        fgets(buffLine, BUFFER_SIZE, filePointer);
        
        //len = strlen(buffLine);

        //fileBuff[i] = (char *) malloc(len * sizeof(char));
        // if (fileBuff[i] == NULL) 
        // {
        //     printf("Could not allocate memory for array[%d]", i);
        //     return 1;
        // }
        strcpy(fileBuff[i], buffLine);
    }

    //check file buffer
    for(int i = 0; i < LineCounter; i++)
    {
        len = strlen(fileBuff[i]);
        printf("\n\n");
        //printf("Line %d Lenght: %d\n", i+1, len);
        printf("Line %d: %s", i+1, fileBuff[i]);  
    }

    //get Maximum ASCII value from each line
    maxValues = malloc(LineCounter * sizeof(int));
    int max;
    int cur;
    for(int i = 0; i < LineCounter; i++)
    {
        len = strlen(fileBuff[i]);
        max = 0;
        for(int j = 0; j < len-1; j++)
        {
            cur = (int) fileBuff[i][j];
            if(cur > max && cur < 123)
            {
                max = cur;
            }
        }
        maxValues[i] = max;
        printf("Line %d Max: %d\n", i+1, max);
        //printf("Line %d: %s", i+1, fileBuff[i]);  
    }    
    
    //check max function
    printf("\n\n");
    getMax(50, 90);

   fclose(filePointer);

    return 0;
}