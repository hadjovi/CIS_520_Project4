#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

int lineNumberTotal, currentLine;

void print_results()
{
  int i,j, total = 0;

  					// then print out the totals
  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     total += char_counts[i];
     printf(" %c %d\n", (char) (i + 97), char_counts[i]);
  }
  printf("\nTotal characters:  %d\n", total);
}

main(int argc, char **argv)
{
    lineNumberTotal = 0, currentLine = 0;
    FILE *filePointer;
    char c; // To store a character read from file
    filePointer = fopen("input.txt", "r");
    if (filePointer == NULL)
    {
        printf("Could not open file ");
        return 0;
    }
    for (c = getc(filePointer); c != EOF; c = getc(filePointer))
    {
        if (c == '\n')
        {
            lineNumberTotal = lineNumberTotal + 1;
        }
    }
    
    fclose(filePointer);

    char buffList[lineNumberTotal];

    //MPI stuff below
    int ierr, rank, n_ranks, lines_per_rank;
    int rank_firstLine, rank_lastLine;
    ierr = MPI_Init(&argc, &argv);

    /* find out MY process ID, and how many processes were started. */

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

    // Calculate the number of iterations for each rank
    lines_per_rank = floor(lineNumberTotal/n_ranks);
    if( lineNumberTotal%n_ranks > 0 ){
        // Add 1 in case the number of ranks doesn't divide the number of lines
        lines_per_rank += 1;
    }
    // Figure out the first and the last line iteration for this rank
    rank_firstLine = rank * lines_per_rank;
    rank_lastLine = rank_firstLine + lines_per_rank;

    // Run only the part of the loop this rank needs to run
    // The if statement makes sure we don't go over
    for( int i = rank_firstLine; i < rank_lastLine; i++ ) {
        if( i < lineNumberTotal ) {
            //add what to do for each line
        }
    }
    //out of the parallel section
    ierr = MPI_Finalize();

    return 0;
}