

#ifndef __GPUTK_ARG_H__
#define __GPUTK_ARG_H__

struct st_gpuTKArg_t {
  char * sessionId;
  int inputCount;
  char **inputFiles;
  char *outputFile;
  char *expectedOutput;
  char *type;
};

#define gpuTKArg_getInputCount(wa) ((wa).inputCount)
#define gpuTKArg_getInputFiles(wa) ((wa).inputFiles)
#define gpuTKArg_getInputFile(wa, ii) (gpuTKArg_getInputFiles(wa)[ii])
#define gpuTKArg_getOutputFile(wa) ((wa).outputFile)
#define gpuTKArg_getSessionId(wa) ((wa).sessionId)
#define gpuTKArg_getExpectedOutputFile(wa) ((wa).expectedOutput)
#define gpuTKArg_getType(wa) ((wa).type)

#define gpuTKArg_setSessionId(wa, val) (gpuTKArg_getSessionId(wa) = val)
#define gpuTKArg_setInputCount(wa, val) (gpuTKArg_getInputCount(wa) = val)
#define gpuTKArg_setInputFiles(wa, val) (gpuTKArg_getInputFiles(wa) = val)
#define gpuTKArg_setInputFile(wa, ii, val) (gpuTKArg_getInputFile(wa, ii) = val)
#define gpuTKArg_setOutputFile(wa, val) (gpuTKArg_getOutputFile(wa) = val)
#define gpuTKArg_setExpectedOutputFile(wa, val)                              \
  (gpuTKArg_getExpectedOutputFile(wa) = val)
#define gpuTKArg_setType(wa, val) (gpuTKArg_getType(wa) = val)

EXTERN_C gpuTKArg_t gpuTKArg_new(int *argc, char ***argv);
EXTERN_C void gpuTKArg_delete(gpuTKArg_t arg);
EXTERN_C gpuTKArg_t gpuTKArg_read(int argc, char **argv);

char* sessionId();
char * _envSessionId();

#endif /* __GPUTK_ARG_H__ */
