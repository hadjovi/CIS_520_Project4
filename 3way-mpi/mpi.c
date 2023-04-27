#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
//--------------- Declare constants here --------------//
#define NUM_RANKS 4
#define BUFFER_SIZE 2005
//--------------- Declare Global Variable here --------------//
int lineNumberTotal, currentLine;
char **fileBuff;

int *maxValues;
int RankNumber;

//--------------- Declare Functions Prototype here --------------//
void getMax(int, int);

void getLine(FILE*);

void copyFile(FILE *filePointer);

void printBuffer();

void getAllMax();

void RankWork();

void printMaxValues();

main(int argc, char **argv)
{
    lineNumberTotal = 0, currentLine = 0;

    //open file
    FILE *filePointer;
    filePointer = fopen("../wiki_100.txt", "r");

    //get the number of lines and update counter
    getLine(filePointer);

    //copy file into file buffer
    copyFile(filePointer);

    //close the file
    fclose(filePointer);
    
    //get Maximum ASCII value from each line
    maxValues = malloc(lineNumberTotal * sizeof(int));


    /* Process work is below*/
    int ierr, rank;
    int rank_firstLine, rank_lastLine;
    ierr = MPI_Init(&argc, &argv);

    //Find out how many processes were started.
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &RankNumber);

    //out of the parallel section
    ierr = MPI_Finalize();

    return 0;
}

//------------------------- Declare Functions here ------------------------//
void getLine(FILE *filePointer)
{
    //Check if file opened successfully
    if (filePointer == NULL) 
    {
        printf("Could not open file\n");
        return;
    }

    //Create a temporary buffer to get the data
    char buffLine[BUFFER_SIZE];

    //read file until end of stream -- get the Counter
    while(fgets(buffLine, BUFFER_SIZE, filePointer))
    {
        //update Counter
        lineNumberTotal++;
    }
    rewind(filePointer);

    printf("Number of Lines : %d\n\n", lineNumberTotal);
}
void copyFile(FILE *filePointer)
{
    int i;

    //copy data to a file buffer
    
    fileBuff = malloc(lineNumberTotal * sizeof(char *));       

    if (fileBuff == NULL) 
    {
        printf("Could not allocate memory for array\n");
        return;
    }

    for(i = 0; i < lineNumberTotal; i++)
    {
        fileBuff[i] = (char *) malloc(BUFFER_SIZE * sizeof(char));

        if (fileBuff[i] == NULL) 
        {
            printf("Could not allocate memory for array[%d]", i);
            return;
        } 
    } 

    //Create a temporary buffer to get the data
    char buffLine[BUFFER_SIZE];

    // Read file line by line and store each line in the array
    for (i = 0; i < lineNumberTotal; i++) 
    {
        fgets(buffLine, BUFFER_SIZE, filePointer);
        strcpy(fileBuff[i], buffLine);
    }
}

void printBuffer()
{
    int i, len;

    //prints file buffer
    for(i = 0; i < lineNumberTotal; i++)
    {
        len = strlen(fileBuff[i]);
        printf("\n\n");
        printf("Line %d: %s", i+1, fileBuff[i]);  
    }
}

void printMaxValues()
{
    int i;

    //prints file buffer
    for(i = 0; i < lineNumberTotal; i++)
    {
        printf("Line %d Max: %d\n", i+1, maxValues[i]); 
    }
}

void RankWork()
{
    int lines_per_rank, remainder, myRank, startPos, endPos, i, j, hs;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    hs = -1;
        
    startPos = myRank * (lineNumberTotal / RankNumber);
	endPos = startPos + (lineNumberTotal / RankNumber);
		
	if(myRank == RankNumber - 1){
		endPos = endPos + lineNumberTotal % RankNumber;
	}

    for(i = startPos; i < endPos; i++)
    {
        for(j = 0; j < BUFFER_SIZE; j++)
        {
            if(fileBuff[i][j] > hs)
            {
                hs = fileBuff[i][j];
            }
        }
        maxValues[i] = hs;
        hs = -2;
    }
}
