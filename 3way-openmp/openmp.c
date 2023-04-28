#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "sys/types.h"
#include "sys/sysinfo.h"


//X Number of File Lines
#define LINES 1000001
#define LINE_LENGTH 2001

struct timeval t1, t2;

typedef struct {
	uint32_t virtualMem;
	uint32_t physicalMem;
} processMem_t;

char buffers[LINES][LINE_LENGTH];
int hsSave[LINES];
int numLines = 0;
int NUM_THREADS = 1;
double elapsedTime;
int i,j;


//Do the printing
void outPrint(){
    //for(i = 0; i < numLines; i++){
    for(i = 0; i < 100; i++){
        printf("%d: %d\n", i , hsSave[i]);
    }
}


//Front Runner
//Fills ptr_array with newlines
//Updates numLines
void frontRunner(){
    FILE * fp = fopen("/homes/dan/625/wiki_dump.txt", "r");
    
    //Iterate through the entire file, fill 'Line Arrays' as it goes
    while(1){
        //If the files is done exit
        if(feof(fp)) { 
            break;
        }
        //Grab the next character
        char c = fgetc(fp);
        buffers[numLines][i] = c;
        i = i + 1;
        //If its an end of the line, handle it!
        if(c == '\n'){
            numLines = numLines + 1;
            i = 0;
        }
    }
    fclose(fp);
}


//Parallel Part
void parallelPart(int myID, int NUM_THREADS){
	int startPos, endPos, i, j, hs;
	
	#pragma omp private(myID,startPos,endPos,i,j,hs)
	{
        hs = -1;
        
        startPos = myID * (LINES / NUM_THREADS);
		endPos = startPos + (LINES / NUM_THREADS);
		
		if(myID == NUM_THREADS - 1){
		    endPos = endPos + LINES % NUM_THREADS;
		}

		printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);
        
        for(i = startPos; i < endPos; i++){
            for(j = 0; j < LINE_LENGTH; j++){
                if(buffers[i][j] > hs){
                    hs = buffers[i][j];
                }
            }
            hsSave[i] = hs;
            //printf("LINE %d: %d\n",i,hs);
            hs = -2;
        }
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

void GetProcessMemory(processMem_t* processMem) {
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


int main(int argc, char *argv[]){
    //start counting the time
    gettimeofday(&t1, NULL);
    
    //Determine Threads by first argument
    int NUM_THREADS = atoi(argv[1]);
    
    processMem_t myMem; 
    
    //Grab info from files
    frontRunner();
    
    //Parallel Part
    
    omp_set_num_threads(NUM_THREADS);
    
    #pragma omp parallel
    {
        parallelPart(omp_get_thread_num(), NUM_THREADS);
    }
    
    outPrint();
    
    //Determine Time
    gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
    printf("Pthread Program:: THREADS: %d, TIME: %f\n", NUM_THREADS, elapsedTime);
    
    GetProcessMemory(&myMem);
    printf("Memory: vMem %u KB, pMem %u KB\n", myMem.virtualMem, myMem.physicalMem);
    return 0;
    
}
