#ifndef __GPUTK_PATH_H__
#define __GPUTK_PATH_H__

char *gpuTKPath_join(const char *p1, const char *p2);
char *gpuTKPath_join(const char *p1, const char *p2, const char *p3);
char *gpuTKPath_join(const char *p1, const char *p2, const char *p3,
                  const char *p4);

char *gpuTKPath_join(const std::string &p1, const std::string &p2);
char *gpuTKPath_join(const std::string &p1, const std::string &p2,
                  const std::string &p3);
char *gpuTKPath_join(const std::string &p1, const std::string &p2,
                  const std::string &p3, const std::string &p4);

template <typename T1, typename T2>
static char *gpuTKPath_join(const T1 &p1, const T2 &p2) {
  return gpuTKPath_join(gpuTKString(p1), gpuTKString(p2));
}
template <typename T1, typename T2, typename T3>
static char *gpuTKPath_join(const T1 &p1, const T2 &p2, const T3 &p3) {
  return gpuTKPath_join(gpuTKString(p1), gpuTKString(p2), gpuTKString(p3));
}
template <typename T1, typename T2, typename T3, typename T4>
static char *gpuTKPath_join(const T1 &p1, const T2 &p2, const T3 &p3,
                         const T4 &p4) {
  return gpuTKPath_join(gpuTKString(p1), gpuTKString(p2), gpuTKString(p3),
                     gpuTKString(p4));
}

#endif /* __GPUTK_PATH_H__ */
