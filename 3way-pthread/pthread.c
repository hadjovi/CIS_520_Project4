#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>

#include "sys/types.h"
#include "sys/sysinfo.h"

//--------------- Declare constants here --------------//
//#define NUM_THREADS 1
#define BUFFER_SIZE 2005

//--------------- Declare Global Variable here --------------//
 
int LineCounter = 0;

char **fileBuff;

int *maxValues;

pthread_mutex_t mutexsum;			// mutex for maxValues

struct timeval t1, t2;

double elapsedTime;

int NUM_THREADS = 1;

typedef  struct 
{
	uint32_t virtualMem;
	uint32_t physicalMem;
} processMem_t;

//--------------- Declare Functions Prototype here --------------//
void getMax(int, int);

void getLine(FILE*);

void copyFile(FILE *filePointer);

void printBuffer();

void getAllMax();

void *getMaxParrallel(void *myID);

void printMaxValues();

void GetProcessMemory(processMem_t* processMem);

//--------------- Main Code starts here --------------//
int main(int argc, char *argv[])
{
    //Determine Threads by first argument
    NUM_THREADS = atoi(argv[1]);
    
    //Set up the pthreads ---------------
    int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    //----------------------------------------------------------//

    //open file
    FILE *filePointer;
    
    //filePointer = fopen("../wiki_100.txt", "r");
    
    filePointer = fopen("/homes/dan/625/wiki_dump.txt", "r");
    
    //get the number of lines and update counter
    getLine(filePointer);

    //copy file into file buffer
    copyFile(filePointer);

    //printBuffer();

    //Close the file
    fclose(filePointer);

    //get Maximum ASCII value from each line
    maxValues = malloc(LineCounter * sizeof(int)); 
    
    //start counting the time
    gettimeofday(&t1, NULL);

    //Start parallel work---------------------------//
    for (i = 0; i < NUM_THREADS; i++ ) 
    {
	      rc = pthread_create(&threads[i], &attr, getMaxParrallel, (void *)i);
	      if (rc) 
          {
	        printf("ERROR; return code from pthread_create() is %d\n", rc);
		    exit(-1);
	      }
	}      

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(i = 0; i < NUM_THREADS; i++) 
    {
	     rc = pthread_join(threads[i], &status);
	     if (rc) 
         {
		   printf("ERROR; return code from pthread_join() is %d\n", rc);
		   exit(-1);
	     }
	}
	
	//stop the timer
    gettimeofday(&t2, NULL);
 
    printf("\n"); 
    //printMaxValues();

    //Get process memory
    processMem_t myMem; 

	GetProcessMemory(&myMem);
	
    //print time
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("Pthread Program:: THREADS: %d,TIME: %f\n", NUM_THREADS, elapsedTime);
	printf("Memory: vMem %u KB, pMem %u KB\n", myMem.virtualMem, myMem.physicalMem);
	
    pthread_mutex_destroy(&mutexsum);
	printf("Main: program completed. Exiting.\n");
	printf("\n\n");
	pthread_exit(NULL);   

    

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

    int len;

    //read file until end of stream -- get the Counter
    while(fgets(buffLine, BUFFER_SIZE, filePointer))
    {
        //update Counter
        LineCounter++;        
    }
    rewind(filePointer);
    
    //printf("\n\n");
    
    //printf("Number of Lines : %d\n\n", LineCounter);
}

void copyFile(FILE *filePointer)
{
    int i, len;

    //copy data to a file buffer
    
    fileBuff = malloc(LineCounter * sizeof(char *));       

    if (fileBuff == NULL) 
    {
        printf("Could not allocate memory for array\n");
        return;
    }

    //Create a temporary buffer to get the data
    char buffLine[BUFFER_SIZE];

    // Read file line by line and store each line in the array
    for (i = 0; i < LineCounter; i++) 
    {      
        //get line into buffer
        fgets(buffLine, BUFFER_SIZE, filePointer);

        //get line size
        len = strlen(buffLine) + 1;

        //allocate space for line
        fileBuff[i] = (char *) malloc(len * sizeof(char));
        if (fileBuff[i] == NULL) 
        {
            printf("Could not allocate memory for array[%d]", i);
            return;
        } 

        //copy line to file buffer
        strcpy(fileBuff[i], buffLine);
    }
}

void printBuffer()
{
    int i, len;

    //prints file buffer
    for(i = 0; i < LineCounter; i++)
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
    for(i = 0; i < LineCounter; i++)
    {
        printf("Line %d Max: %d\n", i+1, maxValues[i]); 
    }
}

void *getMaxParrallel(void *myID)
{
    int startPos = ((int) myID) * (LineCounter / NUM_THREADS);
    int endPos = startPos + (LineCounter / NUM_THREADS);

    if(((int) myID) == NUM_THREADS - 1)
    {
        endPos = endPos + LineCounter % NUM_THREADS;
    }
    printf("myID = %d startPos = %d endPos = %d \n", (int) myID, startPos, endPos);

    int len;
    int max;
    int cur;
    int i , j;

    pthread_mutex_lock (&mutexsum);
    for(i = startPos; i < endPos; i++)
    {
        len = strlen(fileBuff[i]);
        max = 0;
        for(j = 0; j < len-1; j++)
        {
            cur = (int) fileBuff[i][j];
            if(cur > max)
            {
                max = cur;
            }
        }
        maxValues[i] = max;  
    }
    pthread_mutex_unlock (&mutexsum);    

    pthread_exit(NULL);
}

int parseLine(char *line) 
{
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
		//printf("%s", line);
		if (strncmp(line, "VmSize:", 7) == 0) {
			processMem->virtualMem = parseLine(line);
		}

		if (strncmp(line, "VmRSS:", 6) == 0) {
			processMem->physicalMem = parseLine(line);
		}
	}
	fclose(file);
}