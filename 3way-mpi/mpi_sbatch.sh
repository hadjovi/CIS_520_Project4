#!/bin/bash -l
##$ -l h_rt=0:01:00
SBATCH --constraint=moles

module load foss

mpirun -np 1 /homes/garrettrh/MPI/mpi #change to match the path to your code
