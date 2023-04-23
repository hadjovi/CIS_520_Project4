#ifndef OPENMP_H_ 
#define OPENPM_H_

//Function to print out all the "High Score" values from the array
void outPrint();

//Function to open and run through the file char by char and separate out all the lines
void frontRunner();


//Function to look at specific lines and save the highest char value of that line to an array
void parallelPart(int myID);

#endif