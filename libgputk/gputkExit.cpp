
#include "gputk.h"

enum {
  gpuTKMPI_timerTag          = 2,
  gpuTKMPI_loggerTag         = 4,
  gpuTKMPI_solutionExistsTag = 8,
  gpuTKMPI_solutionTag       = 16
};

void gpuTK_atExit(void) {
  using std::cout;
  using std::endl;

#ifdef GPUTK_USE_CUDA
  // cudaDeviceSynchronize();
#endif /* GPUTK_USE_CUDA */

  int nranks = rankCount();
  if (nranks > 1) {
#ifdef GPUTK_USE_MPI
    if (isMasterQ) {
#ifdef gpuTKLogger_printOnExit
      cout << "==$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;

      cout << "{\n";
      cout << gpuTKString_quote("timer") << ":";
      cout << "[\n";
      for (int ii = 0; ii < nranks; ii++) {
        if (ii == 0) {
          cout << gpuTKTimer_toJSON();
        } else {
          const char *msg = gpuTKMPI_getStringFromRank(ii, gpuTKMPI_timerTag);
          if (msg != nullptr && strlen(msg) != 0) {
            cout << ",\n";
            cout << msg;
            //		 free(msg);
          }
        }
      }
      cout << "]" << endl; // close timer

      cout << "," << endl; // start logger
      cout << gpuTKString_quote("logger") << ":";
      cout << "[\n";
      for (int ii = 0; ii < nranks; ii++) {
        if (ii == 0) {
          cout << gpuTKLogger_toJSON();
        } else {
          const char *msg = gpuTKMPI_getStringFromRank(ii, gpuTKMPI_loggerTag);
          if (msg != nullptr && strlen(msg) != 0) {
            cout << ",\n";
            cout << msg;
            //		 free(msg);
          }
        }
      }
      cout << "]" << endl; // close logger

      cout << "," << endl; // start solutionExists
      cout << gpuTKString_quote("cuda_memory") << ":" << _cudaMallocSize
           << ",\n";

      if (solutionJSON) {
        cout << gpuTKString_quote("solution_exists") << ": true,\n";
        cout << gpuTKString_quote("solution") << ":" << solutionJSON << "\n";
      } else {
        cout << gpuTKString_quote("solution_exists") << ": false\n";
      }
      cout << "}" << endl; // close json

    } else {
      gpuTKMPI_sendStringToMaster(gpuTKTimer_toJSON().c_str(), gpuTKMPI_timerTag);
      gpuTKMPI_sendStringToMaster(gpuTKLogger_toJSON().c_str(), gpuTKMPI_loggerTag);
    }
#endif /* gpuTKLogger_printOnExit */

#endif /* GPUTK_USE_MPI */
  } else {
#ifdef gpuTKLogger_printOnExit
    cout << "==$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;

    cout << "{\n" << gpuTKString_quote("timer") << ":[" << gpuTKTimer_toJSON()
         << "],\n" << gpuTKString_quote("logger") << ":[" << gpuTKLogger_toJSON()
         << "],\n";

#ifdef GPUTK_USE_CUDA
    cout << gpuTKString_quote("cuda_memory") << ":" << _cudaMallocSize
         << ",\n";
#endif /* GPUTK_USE_CUDA */

    if (solutionJSON) {
      cout << gpuTKString_quote("solution_exists") << ": true,\n";
      cout << gpuTKString_quote("solution") << ":" << solutionJSON << "\n";
    } else {
      cout << gpuTKString_quote("solution_exists") << ": false\n";
    }
    cout << "}" << endl;
#endif /* gpuTKLogger_printOnExit */
  }

  // gpuTKTimer_delete(_timer);
  // gpuTKLogger_delete(_logger);

  _timer  = nullptr;
  _logger = nullptr;

// gpuTKFile_atExit();

#ifdef GPUTK_USE_CUDA
  cudaDeviceReset();
#endif

  exit(0);

  // assert(0);

  return;
}
