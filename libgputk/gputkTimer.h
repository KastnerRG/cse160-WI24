

#ifndef __GPUTK_TIMER_H__
#define __GPUTK_TIMER_H__

#ifdef GPUTK_USE_WINDOWS
extern uint64_t _hrtime_frequency;
#endif /* _WIN32 */

extern gpuTKTimer_t _timer;

typedef enum en_gpuTKTimerKind_t {
  gpuTKTimerKind_Generic,
  gpuTKTimerKind_IO,
  gpuTKTimerKind_GPU,
  gpuTKTimerKind_Copy,
  gpuTKTimerKind_Driver,
  gpuTKTimerKind_CopyAsync,
  gpuTKTimerKind_Compute,
  gpuTKTimerKind_CPUGPUOverlap,
} gpuTKTimerKind_t;

struct st_gpuTKTimerNode_t {
  int idx;
  char * id;
  char * session_id;
  int mpiRank;
  int level;
  gpuTKBool stoppedQ;
  gpuTKTimerKind_t kind;
  uint64_t startTime;
  uint64_t endTime;
  uint64_t elapsedTime;
  int startLine;
  int endLine;
  const char *startFunction;
  const char *endFunction;
  const char *startFile;
  const char *endFile;
  gpuTKTimerNode_t next;
  gpuTKTimerNode_t prev;
  gpuTKTimerNode_t parent;
  char *msg;
};

struct st_gpuTKTimer_t {
  char * id;
  char * session_id;
  size_t length;
  gpuTKTimerNode_t head;
  gpuTKTimerNode_t tail;
  uint64_t startTime;
  uint64_t endTime;
  uint64_t elapsedTime;
};

#define gpuTKTimerNode_getIdx(node) ((node)->idx)
#define gpuTKTimerNode_getId(node) ((node)->id)
#define gpuTKTimerNode_getSessionId(node) ((node)->session_id)
#define gpuTKTimerNode_getMPIRank(node) ((node)->mpiRank)
#define gpuTKTimerNode_getLevel(node) ((node)->level)
#define gpuTKTimerNode_getStoppedQ(node) ((node)->stoppedQ)
#define gpuTKTimerNode_getKind(node) ((node)->kind)
#define gpuTKTimerNode_getStartTime(node) ((node)->startTime)
#define gpuTKTimerNode_getEndTime(node) ((node)->endTime)
#define gpuTKTimerNode_getElapsedTime(node) ((node)->elapsedTime)
#define gpuTKTimerNode_getStartLine(node) ((node)->startLine)
#define gpuTKTimerNode_getEndLine(node) ((node)->endLine)
#define gpuTKTimerNode_getStartFunction(node) ((node)->startFunction)
#define gpuTKTimerNode_getEndFunction(node) ((node)->endFunction)
#define gpuTKTimerNode_getStartFile(node) ((node)->startFile)
#define gpuTKTimerNode_getEndFile(node) ((node)->endFile)
#define gpuTKTimerNode_getNext(node) ((node)->next)
#define gpuTKTimerNode_getPrevious(node) ((node)->prev)
#define gpuTKTimerNode_getParent(node) ((node)->parent)
#define gpuTKTimerNode_getMessage(node) ((node)->msg)

#define gpuTKTimerNode_setIdx(node, val) (gpuTKTimerNode_getIdx(node) = val)
#define gpuTKTimerNode_setId(node, val) (gpuTKTimerNode_getId(node) = val)
#define gpuTKTimerNode_setSessionId(node, val)                               \
  (gpuTKTimerNode_getSessionId(node) = val)
#define gpuTKTimerNode_setMPIRank(node, val)                                 \
  (gpuTKTimerNode_getMPIRank(node) = val)
#define gpuTKTimerNode_setLevel(node, val) (gpuTKTimerNode_getLevel(node) = val)
#define gpuTKTimerNode_setStoppedQ(node, val)                                \
  (gpuTKTimerNode_getStoppedQ(node) = val)
#define gpuTKTimerNode_setKind(node, val) (gpuTKTimerNode_getKind(node) = val)
#define gpuTKTimerNode_setStartTime(node, val)                               \
  (gpuTKTimerNode_getStartTime(node) = val)
#define gpuTKTimerNode_setEndTime(node, val)                                 \
  (gpuTKTimerNode_getEndTime(node) = val)
#define gpuTKTimerNode_setElapsedTime(node, val)                             \
  (gpuTKTimerNode_getElapsedTime(node) = val)
#define gpuTKTimerNode_setStartLine(node, val)                               \
  (gpuTKTimerNode_getStartLine(node) = val)
#define gpuTKTimerNode_setEndLine(node, val)                                 \
  (gpuTKTimerNode_getEndLine(node) = val)
#define gpuTKTimerNode_setStartFunction(node, val)                           \
  (gpuTKTimerNode_getStartFunction(node) = val)
#define gpuTKTimerNode_setEndFunction(node, val)                             \
  (gpuTKTimerNode_getEndFunction(node) = val)
#define gpuTKTimerNode_setStartFile(node, val)                               \
  (gpuTKTimerNode_getStartFile(node) = val)
#define gpuTKTimerNode_setEndFile(node, val)                                 \
  (gpuTKTimerNode_getEndFile(node) = val)
#define gpuTKTimerNode_setNext(node, val) (gpuTKTimerNode_getNext(node) = val)
#define gpuTKTimerNode_setPrevious(node, val)                                \
  (gpuTKTimerNode_getPrevious(node) = val)
#define gpuTKTimerNode_setParent(node, val)                                  \
  (gpuTKTimerNode_getParent(node) = val)
#define gpuTKTimerNode_setMessage(node, val)                                 \
  (gpuTKTimerNode_getMessage(node) = val)

#define gpuTKTimerNode_stoppedQ(node)                                        \
  (gpuTKTimerNode_getStoppedQ(node) == gpuTKTrue)
#define gpuTKTimerNode_hasNext(node) (gpuTKTimerNode_getNext(node) != nullptr)
#define gpuTKTimerNode_hasPrevious(node)                                     \
  (gpuTKTimerNode_getPrevious(node) != nullptr)
#define gpuTKTimerNode_hasParent(node) (gpuTKTimerNode_getParent(node) != nullptr)

uint64_t _hrtime(void);

gpuTKTimer_t gpuTKTimer_new(void);
void gpuTKTimer_delete(gpuTKTimer_t timer);

string gpuTKTimer_toJSON(gpuTKTimer_t timer);
string gpuTKTimer_toJSON();

string gpuTKTimer_toXML(gpuTKTimer_t timer);
string gpuTKTimer_toXML();

gpuTKTimerNode_t gpuTKTimer_start(gpuTKTimerKind_t kind, const char *file,
                            const char *fun, int line);
gpuTKTimerNode_t gpuTKTimer_start(gpuTKTimerKind_t kind, string msg,
                            const char *file, const char *fun, int line);
void gpuTKTimer_stop(gpuTKTimerKind_t kind, string msg, const char *file,
                  const char *fun, int line);
void gpuTKTimer_stop(gpuTKTimerKind_t kind, const char *file, const char *fun,
                  int line);

#define gpuTKTime_start(kind, ...)                                           \
  gpuTKTimer_start(gpuTKTimerKind_##kind, gpuTKString(__VA_ARGS__), gpuTKFile,        \
                gpuTKFunction, gpuTKLine)
#define gpuTKTime_stop(kind, ...)                                            \
  gpuTKTimer_stop(gpuTKTimerKind_##kind, gpuTKString(__VA_ARGS__), gpuTKFile,         \
               gpuTKFunction, gpuTKLine)

#endif /* __GPUTK_TIMER_H__ */
