#PBS -q batch
#PBS -N out
#PBS -l nodes=1:ppn=8
#PBS -M mneira011@gmail.com
#PBS -m abe

module load openmpi/1.8.5
cd $PBS_O_WORKDIR
NPROCS=`wc -l < $PBS_NODEFILE`
mpiexec -v -n $NPROCS ./placas.x