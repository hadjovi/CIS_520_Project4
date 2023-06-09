#!/bin/bash -l

#SBATCH --time=0:01:00
#SBATCH --constraint=moles
#SBATCH --mem=2G
#SBATCH --switches=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4

host=`echo $SLURM_JOB_NODELIST | sed s/[^a-z0-9]/\ /g | cut -f 1 -d ' '`
nprocs=$SLURM_NTASKS
opts="--printhostnames --quick --pert 3"

echo "*******************************************************************"
echo "Running on $SLURM_NNODES nodes $nprocs cores on nodes $SLURM_JOB_NODELIST"
echo "*******************************************************************"

./pthread 1
./pthread 2
./pthread 4
./pthread 8
./pthread 10
./pthread 15
./pthread 20
./pthread 30
./pthread 40
