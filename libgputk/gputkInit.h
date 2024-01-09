

#ifndef __GPUTK_INIT_H__
#define __GPUTK_INIT_H__

#ifndef _MSC_VER
__attribute__((__constructor__))
#endif /* _MSC_VER */
void gpuTK_init(int *argc, char ***argv);

#endif /* __GPUTK_INIT_H__ */
