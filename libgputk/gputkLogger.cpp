
#include "gputk.h"

gpuTKLogger_t _logger = nullptr;

static inline gpuTKBool gpuTKLogEntry_hasNext(gpuTKLogEntry_t elem) {
  return gpuTKLogEntry_getNext(elem) != nullptr;
}

static inline gpuTKLogEntry_t gpuTKLogEntry_new() {
  gpuTKLogEntry_t elem;

  elem = gpuTKNew(struct st_gpuTKLogEntry_t);

  gpuTKLogEntry_setId(elem, uuid());
  gpuTKLogEntry_setSessionId(elem, sessionId());
  gpuTKLogEntry_setMessage(elem, NULL);
  gpuTKLogEntry_setMPIRank(elem, gpuTKMPI_getRank());
  gpuTKLogEntry_setTime(elem, _hrtime());

  gpuTKLogEntry_setLevel(elem, gpuTKLogLevel_TRACE);

  gpuTKLogEntry_setNext(elem, NULL);

  gpuTKLogEntry_setLine(elem, -1);
  gpuTKLogEntry_setFile(elem, NULL);
  gpuTKLogEntry_setFunction(elem, NULL);

  return elem;
}

static inline gpuTKLogEntry_t
gpuTKLogEntry_initialize(gpuTKLogLevel_t level, string msg, const char *file,
                      const char *fun, int line) {
  gpuTKLogEntry_t elem;

  elem = gpuTKLogEntry_new();

  gpuTKLogEntry_setLevel(elem, level);

  gpuTKLogEntry_setMessage(elem, gpuTKString_duplicate(msg));

  gpuTKLogEntry_setLine(elem, line);
  gpuTKLogEntry_setFile(elem, file);
  gpuTKLogEntry_setFunction(elem, fun);

  return elem;
}

static inline void gpuTKLogEntry_delete(gpuTKLogEntry_t elem) {
  if (elem != nullptr) {
    if (gpuTKLogEntry_getMessage(elem) != nullptr) {
      gpuTKFree(gpuTKLogEntry_getMessage(elem));
    }
    gpuTKDelete(elem);
  }
  return;
}

static inline const char *getLevelName(gpuTKLogLevel_t level) {
  switch (level) {
    case gpuTKLogLevel_unknown:
      return "Unknown";
    case gpuTKLogLevel_OFF:
      return "Off";
    case gpuTKLogLevel_FATAL:
      return "Fatal";
    case gpuTKLogLevel_ERROR:
      return "Error";
    case gpuTKLogLevel_WARN:
      return "Warn";
    case gpuTKLogLevel_INFO:
      return "Info";
    case gpuTKLogLevel_DEBUG:
      return "Debug";
    case gpuTKLogLevel_TRACE:
      return "Trace";
  }
  return NULL;
}

static inline json11::Json gpuTKLogEntry_toJSONObject(gpuTKLogEntry_t elem) {
  if (elem == nullptr) {
    return json11::Json{};
  }
  json11::Json json = json11::Json::object{
      {"id", gpuTKLogEntry_getId(elem)},
      {"session_id", gpuTKLogEntry_getSessionId(elem)},
      {"mpi_rank", gpuTKLogEntry_getMPIRank(elem)},
      {"level", getLevelName(gpuTKLogEntry_getLevel(elem))},
      {"file", gpuTKLogEntry_getFile(elem)},
      {"function", gpuTKLogEntry_getFunction(elem)},
      {"line", gpuTKLogEntry_getLine(elem)},
      {"time", gpuTKLogEntry_getTime(elem)},
      {"message", gpuTKLogEntry_getMessage(elem)},
  };
  return json;
}

static inline string gpuTKLogEntry_toJSON(gpuTKLogEntry_t elem) {
  if (elem == NULL) {
    return "\"\"";
  } else if (GPUTK_USE_JSON11) {
    json11::Json json = gpuTKLogEntry_toJSONObject(elem);
    return json.string_value();
  } else {
    stringstream ss;

    ss << "{\n";
    ss << gpuTKString_quote("id") << ":"
       << gpuTKString_quote(gpuTKLogEntry_getId(elem)) << ",\n";
    ss << gpuTKString_quote("session_id") << ":"
       << gpuTKString_quote(gpuTKLogEntry_getSessionId(elem)) << ",\n";
    ss << gpuTKString_quote("mpi_rank") << ":"
       << gpuTKString(gpuTKLogEntry_getMPIRank(elem)) << ",\n";
    ss << gpuTKString_quote("level") << ":"
       << gpuTKString_quote(getLevelName(gpuTKLogEntry_getLevel(elem))) << ",\n";
    ss << gpuTKString_quote("message") << ":"
       << gpuTKString_quote(gpuTKLogEntry_getMessage(elem)) << ",\n";
    ss << gpuTKString_quote("file") << ":"
       << gpuTKString_quote(gpuTKLogEntry_getFile(elem)) << ",\n";
    ss << gpuTKString_quote("function") << ":"
       << gpuTKString_quote(gpuTKLogEntry_getFunction(elem)) << ",\n";
    ss << gpuTKString_quote("line") << ":" << gpuTKLogEntry_getLine(elem)
       << ",\n";
    ss << gpuTKString_quote("time") << ":" << gpuTKLogEntry_getTime(elem)
       << "\n";
    ss << "}";

    return ss.str();
  }
  return "";
}

static inline string gpuTKLogEntry_toXML(gpuTKLogEntry_t elem) {
  if (elem != nullptr) {
    stringstream ss;

    ss << "<entry>\n";
    ss << "<type>"
       << "LoggerElement"
       << "</type>\n";
    ss << "<id>" << gpuTKLogEntry_getId(elem) << "</id>\n";
    ss << "<session_id>" << gpuTKLogEntry_getSessionId(elem)
       << "</session_id>\n";
    ss << "<level>" << gpuTKLogEntry_getLevel(elem) << "</level>\n";
    ss << "<message>" << gpuTKLogEntry_getMessage(elem) << "</message>\n";
    ss << "<file>" << gpuTKLogEntry_getFile(elem) << "</file>\n";
    ss << "<function>" << gpuTKLogEntry_getFunction(elem) << "</function>\n";
    ss << "<line>" << gpuTKLogEntry_getLine(elem) << "</line>\n";
    ss << "<time>" << gpuTKLogEntry_getTime(elem) << "</time>\n";
    ss << "</entry>\n";

    return ss.str();
  }
  return "";
}

gpuTKLogger_t gpuTKLogger_new() {
  gpuTKLogger_t logger;

  logger = gpuTKNew(struct st_gpuTKLogger_t);

  gpuTKLogger_setId(logger, uuid());
  gpuTKLogger_setSessionId(logger, sessionId());
  gpuTKLogger_setLength(logger, 0);
  gpuTKLogger_setHead(logger, NULL);

  gpuTKLogger_getLevel(logger) = gpuTKLogLevel_TRACE;

  return logger;
}

static inline void _gpuTKLogger_setLevel(gpuTKLogger_t logger,
                                      gpuTKLogLevel_t level) {
  gpuTKLogger_getLevel(logger) = level;
}

static inline void _gpuTKLogger_setLevel(gpuTKLogLevel_t level) {
  _gpuTKLogger_setLevel(_logger, level);
}

#define gpuTKLogger_setLevel(level) _gpuTKLogger_setLevel(gpuTKLogLevel_##level)

void gpuTKLogger_clear(gpuTKLogger_t logger) {
  if (logger != nullptr) {
    gpuTKLogEntry_t tmp;
    gpuTKLogEntry_t iter;

    iter = gpuTKLogger_getHead(logger);
    while (iter != nullptr) {
      tmp = gpuTKLogEntry_getNext(iter);
      gpuTKLogEntry_delete(iter);
      iter = tmp;
    }

    gpuTKLogger_setLength(logger, 0);
    gpuTKLogger_setHead(logger, NULL);
  }
}

void gpuTKLogger_delete(gpuTKLogger_t logger) {
  if (logger != nullptr) {
    gpuTKLogger_clear(logger);
    gpuTKDelete(logger);
  }
  return;
}

void gpuTKLogger_append(gpuTKLogLevel_t level, string msg, const char *file,
                     const char *fun, int line) {
  gpuTKLogEntry_t elem;
  gpuTKLogger_t logger;

  gpuTK_init(NULL, NULL);

  logger = _logger;

  if (gpuTKLogger_getLevel(logger) < level) {
    return;
  }

  elem = gpuTKLogEntry_initialize(level, msg, file, fun, line);

#ifdef gpuTKLogger_printOnLog
  if (gpuTKLogger_printOnLog) {
    if (level <= gpuTKLogger_getLevel(logger) && elem) {
      json11::Json json = json11::Json::object{
          {"type", "logger"},
          {"id", gpuTKLogEntry_getId(elem)},
          {"session_id", gpuTKLogEntry_getSessionId(elem)},
          {"data", gpuTKLogEntry_toJSONObject(elem)}};
      std::cout << json.dump() << std::endl;
    }
  }
#endif /* gpuTKLogger_printOnLog */

  if (gpuTKLogger_getHead(logger) == nullptr) {
    gpuTKLogger_setHead(logger, elem);
  } else {
    gpuTKLogEntry_t prev = gpuTKLogger_getHead(logger);

    while (gpuTKLogEntry_hasNext(prev)) {
      prev = gpuTKLogEntry_getNext(prev);
    }
    gpuTKLogEntry_setNext(prev, elem);
  }

#if 0
  if (level <= gpuTKLogger_getLevel(logger) && elem) {
    const char *levelName = getLevelName(level);

    fprintf(stderr, "= LOG: %s: %s (In %s:%s on line %d). =\n", levelName,
            gpuTKLogEntry_getMessage(elem), gpuTKLogEntry_getFile(elem),
            gpuTKLogEntry_getFunction(elem), gpuTKLogEntry_getLine(elem));
  }
#endif

  gpuTKLogger_incrementLength(logger);

#ifndef JSON_OUTPUT
  printf("%s\n",gpuTKLogEntry_getMessage(elem));
#endif /* JSON_OUTPUT */

  return;
}

string gpuTKLogger_toJSON() {
  return gpuTKLogger_toJSON(_logger);
}

static json11::Json gpuTKLogger_toJSONObject(gpuTKLogger_t logger) {
  std::vector<json11::Json> elems{};

  if (logger != nullptr) {
    gpuTKLogEntry_t iter;
    stringstream ss;

    for (iter = gpuTKLogger_getHead(logger); iter != nullptr;
         iter = gpuTKLogEntry_getNext(iter)) {
      elems.push_back(gpuTKLogEntry_toJSONObject(iter));
    }
  }
  return json11::Json(elems);
}

string gpuTKLogger_toJSON(gpuTKLogger_t logger) {
  if (logger != nullptr) {
    gpuTKLogEntry_t iter;
    stringstream ss;

    for (iter = gpuTKLogger_getHead(logger); iter != nullptr;
         iter = gpuTKLogEntry_getNext(iter)) {
      ss << gpuTKLogEntry_toJSON(iter);
      if (gpuTKLogEntry_getNext(iter) != nullptr) {
        ss << ",\n";
      }
    }

    return ss.str();
  }
  return "";
}

string gpuTKLogger_toXML() {
  return gpuTKLogger_toXML(_logger);
}

string gpuTKLogger_toXML(gpuTKLogger_t logger) {
  if (logger != nullptr) {
    gpuTKLogEntry_t iter;
    stringstream ss;

    ss << "<logger>\n";
    ss << "<type>"
       << "Logger"
       << "</type>\n";
    ss << "<id>" << gpuTKLogger_getId(logger) << "</id>\n";
    ss << "<session_id>" << gpuTKLogger_getSessionId(logger)
       << "</session_id>\n";
    ss << "<elements>\n";
    for (iter = gpuTKLogger_getHead(logger); iter != nullptr;
         iter = gpuTKLogEntry_getNext(iter)) {
      ss << gpuTKLogEntry_toXML(iter);
    }
    ss << "</elements>\n";
    ss << "</logger>\n";

    return ss.str();
  }
  return "";
}
