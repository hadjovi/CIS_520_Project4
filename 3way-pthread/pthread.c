#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//--------------- Declare constants here --------------//
#define NUM_THREADS 4
#define BUFFER_SIZE 2005

//--------------- Declare Global Variable here --------------//
 
int LineCounter = 0;

char **fileBuff;

int *maxValues;

pthread_mutex_t mutexsum;			// mutex for maxValues

//--------------- Declare Functions Prototype here --------------//
void getMax(int, int);

void getLine(FILE*);

void copyFile(FILE *filePointer);

void printBuffer();

void getAllMax();

void *getMaxParrallel(void *myID);

//--------------- Main Code starts here --------------//
int main()
{    
    //open file
    FILE *filePointer;
    
    filePointer = fopen("../wiki_100.txt", "r");
    
    //get the number of lines and update counter
    getLine(filePointer);

    //copy file into file buffer
    copyFile(filePointer);

    //printBuffer();

    //Close the file
    fclose(filePointer);

    //get Maximum ASCII value from each line
    maxValues = malloc(LineCounter * sizeof(int));

    //Set up the pthreads ---------------
    int i, rc;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    //----------------------------------------------------------//


    //Start parallel work---------------------------//
    for (i = 0; i < NUM_THREADS; i++ ) 
    {
	      rc = pthread_create(&threads[i], &attr, getMaxParrallel, (void *)i);
	      if (rc) {
	        printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	      }
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(i=0; i<NUM_THREADS; i++) {
	     rc = pthread_join(threads[i], &status);
	     if (rc) {
		   printf("ERROR; return code from pthread_join() is %d\n", rc);
		   exit(-1);
	     }
	}
    
    // printf("\n\n");
    // getAllMax();
    // printf("\n\n");
    // getMax(1, 50);
    // getMax(51, 100);

    pthread_mutex_destroy(&mutexsum);
	printf("Main: program completed. Exiting.\n");
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
       
    printf("About to start reading\n");

    int len;

    //read file until end of stream -- get the Counter
    while(fgets(buffLine, BUFFER_SIZE, filePointer))
    {
        //update Counter
        LineCounter++;
        len = strlen(buffLine);
    }
    rewind(filePointer);

    printf("\n Number of Line is %d\n", LineCounter);
}

void copyFile(FILE *filePointer)
{
    //copy data to a file buffer
    
    fileBuff = malloc(LineCounter * sizeof(char *));       

    if (fileBuff == NULL) 
    {
        printf("Could not allocate memory for array\n");
        return;
    }

    for(int i = 0; i < LineCounter; i++)
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
    for (int i = 0; i < LineCounter; i++) 
    {
        fgets(buffLine, BUFFER_SIZE, filePointer);
        strcpy(fileBuff[i], buffLine);
    }
}

void printBuffer()
{
    int len;

    //prints file buffer
    for(int i = 0; i < LineCounter; i++)
    {
        len = strlen(fileBuff[i]);
        printf("\n\n");
        printf("Line %d: %s", i+1, fileBuff[i]);  
    }
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
    }  
}

void getAllMax()
{
    int len;
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
    }  
}

void *getMaxParrallel(void *myID)
{
    int startPos = ((int) myID) * (LineCounter / NUM_THREADS);
    int endPos = startPos + (LineCounter / NUM_THREADS);

    printf("myID = %d startPos = %d endPos = %d \n", (int) myID, startPos, endPos);

    getMax(startPos, endPos);

    pthread_exit(NULL);
}