
#ifndef __GPUTK_MPI_H__
#define __GPUTK_MPI_H__

#ifdef GPUTK_USE_MPI

#include <cstring>
#include <mpi/mpi.h>
#include <string>

#define isMasterQ ((gpuTKMPI_getRank()) == 0)

extern int gpuTKMPI_getRank();

extern int rankCount();

extern const char *gpuTKMPI_getStringFromRank(int rank, int tag);
extern void gpuTKMPI_sendStringToMaster(const char *str, int tag);

extern int gpuTKMPI_Init(int *argc, char ***argv);

extern bool finalizedQ;

extern "C" int gpuTKMPI_Finalize(void);
extern "C" void gpuTKMPI_Exit(void);

#define MPI_Finalize gpuTKMPI_Finalize

#else  /* GPUTK_USE_MPI */
static inline int rankCount() {
  return 1;
}
static inline int gpuTKMPI_getRank() {
  return 0;
}
#endif /* GPUTK_USE_MPI */
#endif /* __GPUTK_MPI_H__ */
