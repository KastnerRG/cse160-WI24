#include "gputk.h"

#ifndef PATH_MAX
#ifdef FILENAME_MAX
#define PATH_MAX FILENAME_MAX
#else /* FILENAME_MAX */
#define PATH_MAX 4096
#endif /* FILENAME_MAX */
#endif /* PATH_MAX */

#ifdef GPUTK_USE_UNIX
const char gpuTKDirectorySeperator = '/';
static char *getcwd_(char *buf, int maxLen) {
  return getcwd(buf, maxLen);
}
static void mkdir_(const char *dir) {
  mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
#else  /* GPUTK_USE_LINUX */
const char gpuTKDirectorySeperator = '\\';
static char *getcwd_(char *buf, int maxLen) {
  return _getcwd(buf, maxLen);
}
static void mkdir_(const char *dir) {
  _mkdir(dir);
}
#endif /* GPUTK_USE_LINUX */

EXTERN_C const char *gpuTKDirectory_create(const char *dir) {
  char tmp[PATH_MAX];
  char *p = nullptr;
  size_t len;

#ifdef GPUTK_USE_WINDOWS
  _snprintf(tmp, sizeof(tmp), "%s", dir);
#else /* GPUTK_USE_WINDOWS */
  snprintf(tmp, sizeof(tmp), "%s", dir);
#endif /* GPUTK_USE_WINDOWS */
  len = strlen(tmp);
  if (tmp[len - 1] == gpuTKDirectorySeperator) {
    tmp[len - 1] = 0;
  }
  for (p = tmp + 1; *p; p++) {
    if (*p == gpuTKDirectorySeperator) {
      *p = 0;
      mkdir_(tmp);
      *p = gpuTKDirectorySeperator;
    }
  }
  mkdir_(tmp);
  return dir;
}

EXTERN_C char *gpuTKDirectory_name(const char *pth0) {
  char *pth = gpuTKString_duplicate(pth0);
  char *p   = strrchr(pth, gpuTKDirectorySeperator);
  if (p) {
    p[0] = 0;
  }
  return pth;
}

EXTERN_C char *gpuTKDirectory_current() {
  char *tmp = gpuTKNewArray(char, PATH_MAX + 1);
  if (getcwd_(tmp, PATH_MAX)) {
    return tmp;
  }

  gpuTKDelete(tmp);

  int error = errno;
  switch (error) {
    case EACCES:
      std::cerr
          << "Cannot get current directory :: access denied. exiting..."
          << std::endl;
      exit(-1);
    case ENOMEM:
      std::cerr << "Cannot get current directory :: insufficient storage. "
                   "exiting..."
                << std::endl;
      exit(-1);
    default:
      std::cerr << "Cannot get current directory :: unrecognised error "
                << error << std::endl;
      exit(-1);
  }
}
