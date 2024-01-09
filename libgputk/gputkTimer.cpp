#include "gputk.h"
#include "gputkLogger.h"

#ifdef GPUTK_USE_WINDOWS
uint64_t _hrtime_frequency = 0;
#endif /* GPUTK_USE_WINDOWS */
gpuTKTimer_t _timer = nullptr;

#ifdef GPUTK_USE_DARWIN
static double o_timebase    = 0;
static uint64_t o_timestart = 0;
#endif /* GPUTK_USE_DARWIN */

uint64_t _hrtime(void) {
#define NANOSEC ((uint64_t)1e9)
#ifdef GPUTK_USE_WINDOWS
  LARGE_INTEGER counter;
  if (!QueryPerformanceCounter(&counter)) {
    return 0;
  }
  return ((uint64_t)counter.LowPart * NANOSEC / _hrtime_frequency) +
         (((uint64_t)counter.HighPart * NANOSEC / _hrtime_frequency)
          << 32);
#else
  struct timespec ts;
#ifdef GPUTK_USE_DARWIN
#define O_NANOSEC (+1.0E-9)
#define O_GIGA UINT64_C(1000000000)
  if (!o_timestart) {
    mach_timebase_info_data_t tb{};
    mach_timebase_info(&tb);
    o_timebase = tb.numer;
    o_timebase /= tb.denom;
    o_timestart = mach_absolute_time();
  }
  double diff = (mach_absolute_time() - o_timestart) * o_timebase;
  ts.tv_sec   = diff * O_NANOSEC;
  ts.tv_nsec  = diff - (ts.tv_sec * O_GIGA);
#undef O_NANOSEC
#undef O_GIGA
#else  /* GPUTK_USE_DARWIN */
  clock_gettime(CLOCK_MONOTONIC, &ts);
#endif /* GPUTK_USE_DARWIN */
  return (((uint64_t)ts.tv_sec) * NANOSEC + ts.tv_nsec);
#endif /* GPUTK_USE_WINDOWS */
#undef NANOSEC
}

static inline uint64_t getTime(void) {
#ifdef GPUTK_USE_CUDA
  cudaDeviceSynchronize();
#endif /* GPUTK_USE_CUDA */
  return _hrtime();
}

static inline gpuTKTimerNode_t gpuTKTimerNode_new(int idx, gpuTKTimerKind_t kind,
                                            const char *file,
                                            const char *fun,
                                            int startLine) {
  gpuTKTimerNode_t node = gpuTKNew(struct st_gpuTKTimerNode_t);
  gpuTKTimerNode_setId(node, uuid());
  gpuTKTimerNode_setIdx(node, idx);
  gpuTKTimerNode_setSessionId(node, sessionId());
  gpuTKTimerNode_setMPIRank(node, gpuTKMPI_getRank());
  gpuTKTimerNode_setLevel(node, 0);
  gpuTKTimerNode_setStoppedQ(node, gpuTKFalse);
  gpuTKTimerNode_setKind(node, kind);
  gpuTKTimerNode_setStartTime(node, 0);
  gpuTKTimerNode_setEndTime(node, 0);
  gpuTKTimerNode_setElapsedTime(node, 0);
  gpuTKTimerNode_setStartLine(node, startLine);
  gpuTKTimerNode_setEndLine(node, 0);
  gpuTKTimerNode_setStartFunction(node, fun);
  gpuTKTimerNode_setEndFunction(node, NULL);
  gpuTKTimerNode_setStartFile(node, file);
  gpuTKTimerNode_setEndFile(node, NULL);
  gpuTKTimerNode_setNext(node, NULL);
  gpuTKTimerNode_setPrevious(node, NULL);
  gpuTKTimerNode_setParent(node, NULL);
  gpuTKTimerNode_setMessage(node, NULL);
  return node;
}

static inline void gpuTKTimerNode_delete(gpuTKTimerNode_t node) {
  if (node != nullptr) {
    if (gpuTKTimerNode_getMessage(node)) {
      gpuTKDelete(gpuTKTimerNode_getMessage(node));
    }
    gpuTKDelete(node);
  }
}

static inline const char *_nodeKind(gpuTKTimerKind_t kind) {
  switch (kind) {
    case gpuTKTimerKind_Generic:
      return "Generic";
    case gpuTKTimerKind_IO:
      return "IO";
    case gpuTKTimerKind_GPU:
      return "GPU";
    case gpuTKTimerKind_Copy:
      return "Copy";
    case gpuTKTimerKind_Driver:
      return "Driver";
    case gpuTKTimerKind_CopyAsync:
      return "CopyAsync";
    case gpuTKTimerKind_Compute:
      return "Compute";
    case gpuTKTimerKind_CPUGPUOverlap:
      return "CPUGPUOverlap";
  }
  return "Undefined";
}

static inline json11::Json gpuTKTimerNode_toJSONObject(gpuTKTimerNode_t node) {
  if (node == nullptr) {
    return json11::Json{};
  }
  int parent_id = gpuTKTimerNode_hasParent(node)
                      ? gpuTKTimerNode_getIdx(gpuTKTimerNode_getParent(node))
                      : -1;
  json11::Json json = json11::Json::object{
      {"id", gpuTKTimerNode_getId(node)},
      {"session_id", gpuTKTimerNode_getSessionId(node)},
      {"idx", gpuTKTimerNode_getIdx(node)},
      {"mpi_rank", gpuTKTimerNode_getMPIRank(node)},
      {"stopped", gpuTKTimerNode_stoppedQ(node)},
      {"kind", _nodeKind(gpuTKTimerNode_getKind(node))},
      {"start_time", gpuTKTimerNode_getStartTime(node)},
      {"end_time", gpuTKTimerNode_getEndTime(node)},
      {"elapsed_time", gpuTKTimerNode_getElapsedTime(node)},
      {"start_line", gpuTKTimerNode_getStartLine(node)},
      {"end_line", gpuTKTimerNode_getEndLine(node)},
      {"start_function", gpuTKTimerNode_getStartFunction(node)},
      {"end_function", gpuTKTimerNode_getEndFunction(node)},
      {"start_file", gpuTKTimerNode_getStartFile(node)},
      {"end_file", gpuTKTimerNode_getEndFile(node)},
      {"parent_id", parent_id},
      {"message", gpuTKTimerNode_getMessage(node)},
  };
  return json;
}

static inline void gpuTKTimerNode_toLogger(gpuTKTimerNode_t node) {
  if (node == nullptr) {
    return ;
  }
  int parent_id = gpuTKTimerNode_hasParent(node)
                      ? gpuTKTimerNode_getIdx(gpuTKTimerNode_getParent(node))
                      : -1;
  gpuTKLog(INFO, "[TIME][", _nodeKind(gpuTKTimerNode_getKind(node)),
           "][", gpuTKTimerNode_getMessage(node), 
           "][", gpuTKTimerNode_getEndFile(node),
           ": ", gpuTKTimerNode_getStartLine(node), "-", gpuTKTimerNode_getEndLine(node),
           "] Elapsed time: ", static_cast<double>(gpuTKTimerNode_getElapsedTime(node)) * 1e-6, " ms");
  return ;
}
  
static inline string gpuTKTimerNode_toJSON(gpuTKTimerNode_t node) {
  if (node == nullptr) {
    return "";
  } else if (GPUTK_USE_JSON11) {
    json11::Json json = gpuTKTimerNode_toJSONObject(node);
    return json.string_value();
  } else {
    stringstream ss;

    ss << "{\n";
    ss << gpuTKString_quote("idx") << ":" << gpuTKTimerNode_getIdx(node)
       << ",\n";
    ss << gpuTKString_quote("id") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getId(node)) << ",\n";
    ss << gpuTKString_quote("session_id") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getSessionId(node)) << ",\n";
    ss << gpuTKString_quote("mpi_rank") << ":" << gpuTKTimerNode_getMPIRank(node)
       << ",\n";
    ss << gpuTKString_quote("stopped") << ":"
       << gpuTKString(gpuTKTimerNode_stoppedQ(node) ? "true" : "false") << ",\n";
    ss << gpuTKString_quote("kind") << ":"
       << gpuTKString_quote(_nodeKind(gpuTKTimerNode_getKind(node))) << ",\n";
    ss << gpuTKString_quote("start_time") << ":"
       << gpuTKTimerNode_getStartTime(node) << ",\n";
    ss << gpuTKString_quote("end_time") << ":" << gpuTKTimerNode_getEndTime(node)
       << ",\n";
    ss << gpuTKString_quote("elapsed_time") << ":"
       << gpuTKTimerNode_getElapsedTime(node) << ",\n";
    ss << gpuTKString_quote("start_line") << ":"
       << gpuTKTimerNode_getStartLine(node) << ",\n";
    ss << gpuTKString_quote("end_line") << ":" << gpuTKTimerNode_getEndLine(node)
       << ",\n";
    ss << gpuTKString_quote("start_function") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getStartFunction(node)) << ",\n";
    ss << gpuTKString_quote("end_function") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getEndFunction(node)) << ",\n";
    ss << gpuTKString_quote("start_file") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getStartFile(node)) << ",\n";
    ss << gpuTKString_quote("end_file") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getEndFile(node)) << ",\n";
    ss << gpuTKString_quote("parent_id") << ":"
       << gpuTKString(gpuTKTimerNode_hasParent(node)
                       ? gpuTKTimerNode_getIdx(gpuTKTimerNode_getParent(node))
                       : -1)
       << ",\n";
    ss << gpuTKString_quote("message") << ":"
       << gpuTKString_quote(gpuTKTimerNode_getMessage(node)) << "\n";
    ss << "}";

    return ss.str();
  }
}

static inline string gpuTKTimerNode_toXML(gpuTKTimerNode_t node) {
  if (node == nullptr) {
    return "";
  } else {
    stringstream ss;

    ss << "<node>\n";
    ss << "<idx>" << gpuTKTimerNode_getIdx(node) << "</id>\n";
    ss << "<id>" << gpuTKTimerNode_getId(node) << "</id>\n";
    ss << "<session_id>" << gpuTKTimerNode_getSessionId(node) << "</id>\n";
    ss << "<stoppedQ>"
       << gpuTKString(gpuTKTimerNode_stoppedQ(node) ? "true" : "false")
       << "</stoppedQ>\n";
    ss << "<kind>" << _nodeKind(gpuTKTimerNode_getKind(node)) << "</kind>\n";
    ss << "<start_time>" << gpuTKTimerNode_getStartTime(node)
       << "</start_time>\n";
    ss << "<end_time>" << gpuTKTimerNode_getEndTime(node) << "</end_time>\n";
    ss << "<elapsed_time>" << gpuTKTimerNode_getElapsedTime(node)
       << "</elapsed_time>\n";
    ss << "<start_line>" << gpuTKTimerNode_getStartLine(node)
       << "</start_line>\n";
    ss << "<end_line>" << gpuTKTimerNode_getEndLine(node) << "</end_line>\n";
    ss << "<start_function>" << gpuTKTimerNode_getStartFunction(node)
       << "</start_function>\n";
    ss << "<end_function>" << gpuTKTimerNode_getEndFunction(node)
       << "</end_function>\n";
    ss << "<start_file>" << gpuTKTimerNode_getStartFile(node)
       << "</start_file>\n";
    ss << "<end_file>" << gpuTKTimerNode_getEndFile(node) << "</end_file>\n";
    ss << "<parent_id>"
       << gpuTKString(gpuTKTimerNode_hasParent(node)
                       ? gpuTKTimerNode_getIdx(gpuTKTimerNode_getParent(node))
                       : -1)
       << "</parent_id>\n";
    ss << "<message>" << gpuTKTimerNode_getMessage(node) << "</message>\n";
    ss << "</node>\n";

    return ss.str();
  }
}

#define gpuTKTimer_getId(timer) ((timer)->id)
#define gpuTKTimer_getSessionId(timer) ((timer)->session_id)
#define gpuTKTimer_getLength(timer) ((timer)->length)
#define gpuTKTimer_getHead(timer) ((timer)->head)
#define gpuTKTimer_getTail(timer) ((timer)->tail)
#define gpuTKTimer_getStartTime(timer) ((timer)->startTime)
#define gpuTKTimer_getEndTime(timer) ((timer)->endTime)
#define gpuTKTimer_getElapsedTime(timer) ((timer)->elapsedTime)

#define gpuTKTimer_setId(timer, val) (gpuTKTimer_getId(timer) = val)
#define gpuTKTimer_setSessionId(timer, val)                                  \
  (gpuTKTimer_getSessionId(timer) = val)
#define gpuTKTimer_setLength(timer, val) (gpuTKTimer_getLength(timer) = val)
#define gpuTKTimer_setHead(timer, val) (gpuTKTimer_getHead(timer) = val)
#define gpuTKTimer_setTail(timer, val) (gpuTKTimer_getTail(timer) = val)
#define gpuTKTimer_setStartTime(node, val) (gpuTKTimer_getStartTime(node) = val)
#define gpuTKTimer_setEndTime(node, val) (gpuTKTimer_getEndTime(node) = val)
#define gpuTKTimer_setElapsedTime(node, val)                                 \
  (gpuTKTimer_getElapsedTime(node) = val)

#define gpuTKTimer_incrementLength(timer) (gpuTKTimer_getLength(timer)++)
#define gpuTKTimer_decrementLength(timer) (gpuTKTimer_getLength(timer)--)

#define gpuTKTimer_emptyQ(timer) (gpuTKTimer_getLength(timer) == 0)

void gpuTKTimer_delete(gpuTKTimer_t timer) {
  if (timer != nullptr) {
    gpuTKTimerNode_t tmp, iter;

    iter = gpuTKTimer_getHead(timer);
    while (iter) {
      tmp = gpuTKTimerNode_getNext(iter);
      gpuTKTimerNode_delete(iter);
      iter = tmp;
    }

    gpuTKDelete(timer);
  }
}

static json11::Json gpuTKTimer_toJSONObject(gpuTKTimer_t timer) {

  stringstream ss;
  gpuTKTimerNode_t iter;
  uint64_t currentTime;
  std::vector<json11::Json> elems;

  currentTime = getTime();

  gpuTKTimer_setEndTime(timer, currentTime);
  gpuTKTimer_setElapsedTime(timer, currentTime - gpuTKTimer_getStartTime(timer));

  for (iter = gpuTKTimer_getHead(timer); iter != nullptr;
       iter = gpuTKTimerNode_getNext(iter)) {
    if (!gpuTKTimerNode_stoppedQ(iter)) {
      gpuTKTimerNode_setEndTime(iter, currentTime);
      gpuTKTimerNode_setElapsedTime(iter, currentTime -
                                           gpuTKTimerNode_getStartTime(iter));
    }
    elems.push_back(gpuTKTimerNode_toJSONObject(iter));
  }
  return json11::Json(elems);
}

string gpuTKTimer_toJSON(gpuTKTimer_t timer) {
  if (timer == nullptr) {
    return "";
  } else if (GPUTK_USE_JSON11) {
    json11::Json json = gpuTKTimer_toJSONObject(timer);
    return json.string_value();
  } else {
    stringstream ss;
    gpuTKTimerNode_t iter;
    uint64_t currentTime;

    currentTime = getTime();

    gpuTKTimer_setEndTime(timer, currentTime);
    gpuTKTimer_setElapsedTime(timer,
                           currentTime - gpuTKTimer_getStartTime(timer));

    for (iter = gpuTKTimer_getHead(timer); iter != nullptr;
         iter = gpuTKTimerNode_getNext(iter)) {
      if (!gpuTKTimerNode_stoppedQ(iter)) {
        gpuTKTimerNode_setEndTime(iter, currentTime);
        gpuTKTimerNode_setElapsedTime(
            iter, currentTime - gpuTKTimerNode_getStartTime(iter));
      }
      ss << gpuTKTimerNode_toJSON(iter);
      if (gpuTKTimerNode_getNext(iter) != nullptr) {
        ss << ",\n";
      }
    }

    return ss.str();
  }
}

string gpuTKTimer_toJSON() {
  return gpuTKTimer_toJSON(_timer);
}

string gpuTKTimer_toXML(gpuTKTimer_t timer) {
  if (timer == nullptr) {
    return "";
  } else {
    stringstream ss;
    gpuTKTimerNode_t iter;
    uint64_t currentTime;

    currentTime = getTime();

    gpuTKTimer_setEndTime(timer, currentTime);
    gpuTKTimer_setElapsedTime(timer,
                           currentTime - gpuTKTimer_getStartTime(timer));

    ss << "<timer>\n";
    ss << "<start_time>" << gpuTKTimer_getStartTime(timer)
       << "</start_time>\n";
    ss << "<end_time>" << gpuTKTimer_getEndTime(timer) << "</end_time>\n";
    ss << "<elapsed_time>" << gpuTKTimer_getElapsedTime(timer)
       << "</elapsed_time>\n";
    ss << "<elements>\n";
    for (iter = gpuTKTimer_getHead(timer); iter != nullptr;
         iter = gpuTKTimerNode_getNext(iter)) {
      if (!gpuTKTimerNode_stoppedQ(iter)) {
        gpuTKTimerNode_setEndTime(iter, currentTime);
        gpuTKTimerNode_setElapsedTime(
            iter, currentTime - gpuTKTimerNode_getStartTime(iter));
      }
      ss << gpuTKTimerNode_toXML(iter);
    }
    ss << "</elements>\n";
    ss << "</timer>\n";

    return ss.str();
  }
}

string gpuTKTimer_toXML() {
  return gpuTKTimer_toXML(_timer);
}

gpuTKTimer_t gpuTKTimer_new(void) {
  gpuTKTimer_t timer = gpuTKNew(struct st_gpuTKTimer_t);
  gpuTKTimer_setId(timer, uuid());
  gpuTKTimer_setSessionId(timer, sessionId());
  gpuTKTimer_setLength(timer, 0);
  gpuTKTimer_setHead(timer, NULL);
  gpuTKTimer_setTail(timer, NULL);
  gpuTKTimer_setStartTime(timer, getTime());
  gpuTKTimer_setEndTime(timer, 0);
  gpuTKTimer_setElapsedTime(timer, 0);

  return timer;
}

static inline gpuTKTimerNode_t _findParent(gpuTKTimer_t timer) {
  gpuTKTimerNode_t iter;

  for (iter = gpuTKTimer_getTail(timer); iter != nullptr;
       iter = gpuTKTimerNode_getPrevious(iter)) {
    if (!gpuTKTimerNode_stoppedQ(iter)) {
      return iter;
    }
  }
  return NULL;
}

static inline void _insertIntoList(gpuTKTimer_t timer, gpuTKTimerNode_t node) {
  if (gpuTKTimer_emptyQ(timer)) {
    gpuTKTimer_setHead(timer, node);
    gpuTKTimer_setTail(timer, node);
  } else {
    gpuTKTimerNode_t end = gpuTKTimer_getTail(timer);
    gpuTKTimer_setTail(timer, node);
    gpuTKTimerNode_setNext(end, node);
    gpuTKTimerNode_setPrevious(node, end);
  }
  gpuTKTimer_incrementLength(timer);
}

gpuTKTimerNode_t gpuTKTimer_start(gpuTKTimerKind_t kind, const char *file,
                            const char *fun, int line) {
  int id;
  uint64_t currentTime;
  gpuTKTimerNode_t node;
  gpuTKTimerNode_t parent;

  // gpuTK_init(NULL, NULL);

  currentTime = getTime();

  id = gpuTKTimer_getLength(_timer);

  node = gpuTKTimerNode_new(id, kind, file, fun, line);

  parent = _findParent(_timer);
  _insertIntoList(_timer, node);

  gpuTKTimerNode_setStartTime(node, currentTime);
  gpuTKTimerNode_setParent(node, parent);
  if (parent != nullptr) {
    gpuTKTimerNode_setLevel(node, gpuTKTimerNode_getLevel(parent) + 1);
  }

  return node;
}

gpuTKTimerNode_t gpuTKTimer_start(gpuTKTimerKind_t kind, string msg,
                            const char *file, const char *fun, int line) {
  gpuTKTimerNode_t node = gpuTKTimer_start(kind, file, fun, line);
  gpuTKTimerNode_setMessage(node, gpuTKString_duplicate(msg));
  return node;
}

static inline gpuTKTimerNode_t _findNode(gpuTKTimer_t timer, gpuTKTimerKind_t kind,
                                      string msg) {
  gpuTKTimerNode_t iter;

  for (iter = gpuTKTimer_getTail(timer); iter != nullptr;
       iter = gpuTKTimerNode_getPrevious(iter)) {
    if (msg == "") {
      if (!gpuTKTimerNode_stoppedQ(iter) &&
          gpuTKTimerNode_getKind(iter) == kind) {
        return iter;
      }
    } else {
      if (!gpuTKTimerNode_stoppedQ(iter) &&
          gpuTKTimerNode_getKind(iter) == kind &&
          msg == gpuTKTimerNode_getMessage(iter)) {
        return iter;
      }
    }
  }
  return NULL;
}

void gpuTKTimer_stop(gpuTKTimerKind_t kind, string msg, const char *file,
                  const char *fun, int line) {
  uint64_t currentTime;
  gpuTKTimerNode_t node;

  currentTime = getTime();

  node = _findNode(_timer, kind, msg);

  gpuTKAssert(node != nullptr);
  if (node == nullptr) {
    return;
  }

  gpuTKTimerNode_setEndTime(node, currentTime);
  gpuTKTimerNode_setElapsedTime(node,
                             currentTime - gpuTKTimerNode_getStartTime(node));
  gpuTKTimerNode_setEndLine(node, line);
  gpuTKTimerNode_setEndFunction(node, fun);
  gpuTKTimerNode_setEndFile(node, file);
  gpuTKTimerNode_setStoppedQ(node, gpuTKTrue);

#if (! defined(gpuTKLogger_printOnLog) ) && ( defined(gpuTKLogger_TimerLog) )
  if(node)
    gpuTKTimerNode_toLogger(node);
#endif
#ifdef gpuTKLogger_printOnLog
  if (gpuTKLogger_printOnLog && node) {
    json11::Json json = json11::Json::object{
        {"type", "timer"},
        {"id", gpuTKTimerNode_getId(node)},
        {"session_id", gpuTKTimerNode_getSessionId(node)},
        {"data", gpuTKTimerNode_toJSONObject(node)}};
    std::cout << json.dump() << std::endl;
  }
#endif /* gpuTKLogger_printOnLog */
  return;
}

void gpuTKTimer_stop(gpuTKTimerKind_t kind, const char *file, const char *fun,
                  int line) {
  gpuTKTimer_stop(kind, "", file, fun, line);
}
