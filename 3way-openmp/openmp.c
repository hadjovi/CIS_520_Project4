#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 25

//X Number of File Lines
#define LINES 1000001
#define LINE_LENGTH 2001

char buffers[LINES][LINE_LENGTH];
int hsSave[LINES];
int numLines = 0;
int i,j;


//Do the printing
void outPrint(){
    /*
    printf("PRINT");
    for(i = 0; i < LINES; i++){
        for(j = 0; j <= LINE_LENGTH; j++){
            if(buffers[i][j] == '\n'){
                printf("\n");
                break;
            }
            printf("%c", buffers[i][j]);
        }
        printf("\n");
    }
    */

    for(i = 0; i < LINES; i++){
        printf("Line %d: %d\n", i , hsSave[i]);
    }
}



//Front Runner
//Fills ptr_array with newlines
//Updates numLines
void frontRunner(){
    //FILE * fp = fopen("../wiki_dump.txt", "r");
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
void parallelPart(int myID){
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
       
        //printf("myID = %d done\n", myID);
        //#pragma omp barrier
    }
}


void main(){
    //Grab info from files
    frontRunner();
    
    //Parallel Part
    
    omp_set_num_threads(NUM_THREADS);
    
    #pragma omp parallel
    {
        parallelPart(omp_get_thread_num());
        //#pragma omp barrier
    }

    outPrint();
}