
#include "gputk.h"

#ifdef GPUTK_USE_SESSION_ID
static char * _sessionId = nullptr;
char * _envSessionId() {
#ifdef GPUTK_USE_UNIX
  if (_sessionId != nullptr) {
    char *env = std::getenv("SESSION_ID");
    if (env) {
      _sessionId = gpuTKString_duplicate(env);
    }
  }
#endif /* GPUTK_USE_UNIX */
  return gpuTKString_duplicate(_sessionId);
}
char * sessionId() {
  if (_sessionId != "") {
    return gpuTKString_duplicate(_sessionId);
  }
  return gpuTKString_duplicate(_envSessionId());
}
#else /* GPUTK_USE_SESSION_ID */
char * _envSessionId() {
  return gpuTKString_duplicate("session_id_disabled");
}
char * sessionId() {
  return gpuTKString_duplicate("session_id_disabled");
}
#endif /* GPUTK_USE_SESSION_ID */

gpuTKArg_t gpuTKArg_new(int *argc, char ***argv) {
  gpuTKArg_t arg;

  gpuTK_init(argc, argv);

  gpuTKArg_setSessionId(arg, _envSessionId());
  gpuTKArg_setInputCount(arg, 0);
  gpuTKArg_setInputFiles(arg, NULL);
  gpuTKArg_setOutputFile(arg, NULL);
  gpuTKArg_setType(arg, NULL);
  gpuTKArg_setExpectedOutputFile(arg, NULL);
  return arg;
}

void gpuTKArg_delete(gpuTKArg_t arg) {
  if (gpuTKArg_getInputCount(arg) > 0 && gpuTKArg_getInputFiles(arg) != nullptr) {
    int ii;
    for (ii = 0; ii < gpuTKArg_getInputCount(arg); ii++) {
      gpuTKDelete(gpuTKArg_getInputFile(arg, ii));
    }
    gpuTKDelete(gpuTKArg_getInputFiles(arg));
    gpuTKArg_setInputCount(arg, 0);
    gpuTKArg_setInputFiles(arg, NULL);
  }
  if (gpuTKArg_getOutputFile(arg)) {
    gpuTKDelete(gpuTKArg_getOutputFile(arg));
    gpuTKArg_setOutputFile(arg, NULL);
  }
  if (gpuTKArg_getExpectedOutputFile(arg)) {
    gpuTKDelete(gpuTKArg_getExpectedOutputFile(arg));
    gpuTKArg_setExpectedOutputFile(arg, NULL);
  }
  if (gpuTKArg_getType(arg)) {
    gpuTKDelete(gpuTKArg_getType(arg));
    gpuTKArg_setType(arg, NULL);
  }
  return;
}

static int getInputFileCount(char *arg) {
  int count = 1;
  while (*arg != '\0' && *arg != '-') {
    if (*arg == ',') {
      count++;
    }
    arg++;
  }
  return count;
}

static char **parseInputFiles(char *arg, int *resCount) {
  int count;
  int ii = 0;
  char **files;
  char *token;

  count = getInputFileCount(arg);

  files = gpuTKNewArray(char *, count);

  token = strtok(arg, ",");
  while (token != nullptr) {
    files[ii++] = gpuTKString_duplicate(token);
    token       = strtok(NULL, ",");
  }
  *resCount = ii;
  return files;
}

static char *parseString(char *arg) {
  return gpuTKString_duplicate(arg);
}

static void parseSessionId(char *arg) {
#ifdef GPUTK_USE_SESSION_ID
  _sessionId = std::string(arg);
#endif /* GPUTK_USE_SESSION_ID */
}

gpuTKArg_t gpuTKArg_read(int argc, char **argv) {
  int ii;
  gpuTKArg_t arg;

  arg = gpuTKArg_new(&argc, &argv);
  for (ii = 0; ii < argc; ii++) {
    if (gpuTKString_startsWith(argv[ii], "-s")) {
      parseSessionId(argv[++ii]);
      gpuTKArg_setSessionId(arg, sessionId());
    } else if (gpuTKString_startsWith(argv[ii], "-i")) {
      int fileCount;
      char **files;

      files = parseInputFiles(argv[++ii], &fileCount);

      gpuTKArg_setInputCount(arg, fileCount);
      gpuTKArg_setInputFiles(arg, files);
    } else if (gpuTKString_startsWith(argv[ii], "-o")) {
      char *file = parseString(argv[++ii]);
      gpuTKArg_setOutputFile(arg, file);
    } else if (gpuTKString_startsWith(argv[ii], "-e")) {
      char *file = parseString(argv[++ii]);
      gpuTKArg_setExpectedOutputFile(arg, file);
    } else if (gpuTKString_startsWith(argv[ii], "-t")) {
      char *type = parseString(argv[++ii]);
      gpuTKArg_setType(arg, type);
    } else if (gpuTKString_startsWith(argv[ii], "-h")){
      gpuTKLog(ERROR, "./gpuTKexample -s <sessionid> -e <file> -i <file1,...> -o <file> -t <type>\n");
    } else if (argv[ii][0] == '-') {
      gpuTKLog(ERROR, "Unexpected program option ", argv[ii]);
    }
  }

  return arg;
}
