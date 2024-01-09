

#ifndef __GPUTK_SOLUTION_H__
#define __GPUTK_SOLUTION_H__

typedef struct st_gpuTKSolution_t {
  char * id;
  char * session_id;
  char *type;
  char *outputFile;
  void *data;
  int rows;
  int columns;
  int depth;
} gpuTKSolution_t;

#define gpuTKSolution_getId(sol) ((sol).id)
#define gpuTKSolution_getSessionId(sol) ((sol).session_id)
#define gpuTKSolution_getType(sol) ((sol).type)
#define gpuTKSolution_getOutputFile(sol) ((sol).outputFile)
#define gpuTKSolution_getData(sol) ((sol).data)
#define gpuTKSolution_getRows(sol) ((sol).rows)
#define gpuTKSolution_getColumns(sol) ((sol).columns)
#define gpuTKSolution_getDepth(sol) ((sol).depth)

#define gpuTKSolution_getHeight gpuTKSolution_getRows
#define gpuTKSolution_getWidth gpuTKSolution_getColumns
#define gpuTKSolution_getChannels gpuTKSolution_getDepth

#define gpuTKSolution_setId(sol, val) (gpuTKSolution_getId(sol) = val)
#define gpuTKSolution_setSessionId(sol, val)                                 \
  (gpuTKSolution_getSessionId(sol) = val)
#define gpuTKSolution_setType(sol, val) (gpuTKSolution_getType(sol) = val)
#define gpuTKSolution_setOutputFile(sol, val)                                \
  (gpuTKSolution_getOutputFile(sol) = val)
#define gpuTKSolution_setData(sol, val) (gpuTKSolution_getData(sol) = val)
#define gpuTKSolution_setRows(sol, val) (gpuTKSolution_getRows(sol) = val)
#define gpuTKSolution_setColumns(sol, val) (gpuTKSolution_getColumns(sol) = val)
#define gpuTKSolution_setDepth(sol, val) (gpuTKSolution_getDepth(sol) = val)

gpuTKBool gpuTKSolution(char *expectedOutputFile, char *outputFile, char *type0,
                  void *data, int rows, int columns);
gpuTKBool gpuTKSolution(gpuTKArg_t arg, void *data, int rows, int columns);
EXTERN_C gpuTKBool gpuTKSolution(gpuTKArg_t arg, void *data, int rows);
gpuTKBool gpuTKSolution(gpuTKArg_t arg, gpuTKImage_t img);

#endif /* __GPUTK_SOLUTION_H__ */
