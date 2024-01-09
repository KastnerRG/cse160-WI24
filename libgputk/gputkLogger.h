
#ifndef __GPUTK_LOGGER_H__
#define __GPUTK_LOGGER_H__

#include <stdint.h>

typedef enum en_gpuTKLogLevel_t {
  gpuTKLogLevel_unknown = -1,
  gpuTKLogLevel_OFF     = 0,
  gpuTKLogLevel_FATAL,
  gpuTKLogLevel_ERROR,
  gpuTKLogLevel_WARN,
  gpuTKLogLevel_INFO,
  gpuTKLogLevel_DEBUG,
  gpuTKLogLevel_TRACE
} gpuTKLogLevel_t;

struct st_gpuTKLogEntry_t {
  char * id;
  char * session_id;
  int line;
  int mpiRank;
  char *msg;
  uint64_t time;
  const char *fun;
  const char *file;
  gpuTKLogLevel_t level;
  gpuTKLogEntry_t next;
};

struct st_gpuTKLogger_t {
  char * id;
  char * session_id;
  int length;
  gpuTKLogEntry_t head;
  gpuTKLogLevel_t level;
};

#define gpuTKLogEntry_getId(elem) ((elem)->id)
#define gpuTKLogEntry_getSessionId(elem) ((elem)->session_id)
#define gpuTKLogEntry_getMessage(elem) ((elem)->msg)
#define gpuTKLogEntry_getMPIRank(elem) ((elem)->mpiRank)
#define gpuTKLogEntry_getTime(elem) ((elem)->time)
#define gpuTKLogEntry_getLevel(elem) ((elem)->level)
#define gpuTKLogEntry_getNext(elem) ((elem)->next)
#define gpuTKLogEntry_getLine(elem) ((elem)->line)
#define gpuTKLogEntry_getFunction(elem) ((elem)->fun)
#define gpuTKLogEntry_getFile(elem) ((elem)->file)

#define gpuTKLogEntry_setId(elem, val) (gpuTKLogEntry_getId(elem) = val)
#define gpuTKLogEntry_setSessionId(elem, val)                                \
  (gpuTKLogEntry_getSessionId(elem) = val)
#define gpuTKLogEntry_setMessage(elem, val)                                  \
  (gpuTKLogEntry_getMessage(elem) = val)
#define gpuTKLogEntry_setMPIRank(elem, val)                                  \
  (gpuTKLogEntry_getMPIRank(elem) = val)
#define gpuTKLogEntry_setTime(elem, val) (gpuTKLogEntry_getTime(elem) = val)
#define gpuTKLogEntry_setLevel(elem, val) (gpuTKLogEntry_getLevel(elem) = val)
#define gpuTKLogEntry_setNext(elem, val) (gpuTKLogEntry_getNext(elem) = val)
#define gpuTKLogEntry_setLine(elem, val) (gpuTKLogEntry_getLine(elem) = val)
#define gpuTKLogEntry_setFunction(elem, val)                                 \
  (gpuTKLogEntry_getFunction(elem) = val)
#define gpuTKLogEntry_setFile(elem, val) (gpuTKLogEntry_getFile(elem) = val)

#define gpuTKLogger_getId(log) ((log)->id)
#define gpuTKLogger_getSessionId(log) ((log)->session_id)
#define gpuTKLogger_getLength(log) ((log)->length)
#define gpuTKLogger_getHead(log) ((log)->head)
#define gpuTKLogger_getLevel(log) ((log)->level)

#define gpuTKLogger_setId(log, val) (gpuTKLogger_getId(log) = val)
#define gpuTKLogger_setSessionId(log, val) (gpuTKLogger_getSessionId(log) = val)
#define gpuTKLogger_setLength(log, val) (gpuTKLogger_getLength(log) = val)
#define gpuTKLogger_setHead(log, val) (gpuTKLogger_getHead(log) = val)

#define gpuTKLogger_incrementLength(log) (gpuTKLogger_getLength(log)++)
#define gpuTKLogger_decrementLength(log) (gpuTKLogger_getLength(log)--)

#define gpuTKLog(level, ...)                                                 \
  gpuTKLogger_append(gpuTKLogLevel_##level, gpuTKString(__VA_ARGS__), gpuTKFile,      \
                  gpuTKFunction, gpuTKLine)

extern gpuTKLogger_t _logger;

gpuTKLogger_t gpuTKLogger_new();

void gpuTKLogger_clear(gpuTKLogger_t logger);

void gpuTKLogger_delete(gpuTKLogger_t logger);

void gpuTKLogger_append(gpuTKLogLevel_t level, string msg, const char *file,
                     const char *fun, int line);

string gpuTKLogger_toXML(gpuTKLogger_t logger);
string gpuTKLogger_toXML();

string gpuTKLogger_toJSON(gpuTKLogger_t logger);
string gpuTKLogger_toJSON();

#endif /* __GPUTK_LOGGER_H__ */
