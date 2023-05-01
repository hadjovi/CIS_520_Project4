vim#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>

#include "sys/types.h"
#include "sys/sysinfo.h"
//--------------- Declare constants here --------------//
#define BUFFER_SIZE 2005

//--------------- Timing and memory values and struct here --------------//
struct timeval t1, t2;

typedef struct {
	uint32_t virtualMem;
	uint32_t physicalMem;
} processMem_t;

//--------------- Declare Global Variable here --------------//
int lineNumberTotal = 0;
int *maxValues;
int *toSend;
int RankNumber;
int linesPerRank;
double elapsedTime;

//--------------- Declare Functions Prototype here --------------//

void getLine(FILE*);

int *RankWork(void *rank, FILE *filePointer);

void printMaxValues();

int parseLine(char *line);

void GetProcessMemory(processMem_t* processMem);

int GetMaxValue(char * list);

int main(int argc, char **argv)
{
    /* Process work is below*/
    //process start
    int ierr, rank, numtasks;
    ierr = MPI_Init(&argc, &argv);
    if(ierr != MPI_SUCCESS){
        printf("Error starting MPI program \n");
            MPI_Abort(MPI_COMM_WORLD, ierr);
    }
    //get ranks and size
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    RankNumber = numtasks;
    printf("size = %d rank = %d\n", RankNumber, rank); fflush(stdout);
     //open file
    FILE *filePointer;
    filePointer = fopen("/homes/dan/625/wiki_dump.txt", "r");
    
    if(rank == 0){
        //Start timer
        gettimeofday(&t1, NULL);
        
        //get the number of lines and update counter
        getLine(filePointer);
        //allocate final array
        maxValues = malloc((lineNumberTotal + 1) * sizeof(int));
        
        //close the file
    }
    
    //cast line number total
    MPI_Bcast(&lineNumberTotal, 1, MPI_INT, 0, MPI_COMM_WORLD);
    toSend = malloc(lineNumberTotal * sizeof(int));
    
    //Do the work
    toSend = RankWork(&rank, filePointer);
    
    //all threads submit parts of final array
    MPI_Gather(toSend, linesPerRank, MPI_INT, maxValues, linesPerRank, MPI_INT, 0, MPI_COMM_WORLD);
    
    //only one rank runs
    if(rank == 0){
	 //use to see the values
	 printMaxValues()   
	 
        //Determine Time
        gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
        printf("MPI Program:: RANKS: %d, TIME: %f\n", RankNumber, elapsedTime); fflush(stdout);
    
        //Determine Memory
        processMem_t myMem;
        GetProcessMemory(&myMem);
        printf("Memory: vMem %u KB, pMem %u KB\n", myMem.virtualMem, myMem.physicalMem); fflush(stdout);
    }
    //Stop MPI
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
    
}

int *RankWork(void *rank, FILE *filePointer)
{
    int lineToStore, myRank, startPos, endPos, i;
    myRank = *((int*) rank);
    startPos = ((long)myRank) * (lineNumberTotal / RankNumber);
	endPos = startPos + (lineNumberTotal / RankNumber) - 1;
	linesPerRank = endPos - startPos + 1;
	int *localMaxValues = malloc((linesPerRank) * sizeof(int));
	if(rank == 0){
	}
    
    //Create a temporary buffer to get the data
    char *buffLine = calloc(BUFFER_SIZE, sizeof(char));
    // Read file line by line and store each line in the array
    for (i = 0; i < lineNumberTotal; i++) 
    {   
        if(i >= startPos && i <= endPos){
            //get line into buffer
            fgets(buffLine, BUFFER_SIZE, filePointer);
            lineToStore = (i - startPos);
            localMaxValues[lineToStore] = GetMaxValue(buffLine);
        }
    }
    return localMaxValues;
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

int parseLine(char *line) {
	// This assumes that a digit will be found and the line ends in " Kb".
	int i = strlen(line);
	const char *p = line;
	while (*p < '0' || *p > '9') p++;
	line[i - 3] = '\0';
	i = atoi(p);
	return i;
}

void GetProcessMemory(processMem_t* processMem) 
{
    FILE *file = fopen("/proc/self/status", "r");
    char line[128];

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            processMem->virtualMem = parseLine(line);
        }

        if (strncmp(line, "VmRSS:", 6) == 0) {
            processMem->physicalMem = parseLine(line);
        }
    }
    fclose(file);
}


int GetMaxValue(char * list)
{
    int hs = -1;
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        if(list[i] > hs)
        {
            hs = list[i];
        }
    }
    return hs;
}
