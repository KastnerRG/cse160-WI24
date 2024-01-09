

#ifndef __GPUTK_H__
#define __GPUTK_H__

#define gpuTKLogger_TimerLog

/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _MSC_VER

// set minimal warning level
#pragma warning(push, 0)
// some warnings still occur at this level
// if necessary, disable specific warnings not covered by previous pragma
#pragma warning(                                                          \
    disable : 4244 4056 4305 4800 4267 4996 4756 4661 4385 4101 4800)

#define NOMINMAX // do not define min/max in the standard headers

#define __func__ __FUNCTION__
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif /* _CRT_SECURE_NO_WARNINGS */
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#include <direct.h>
#include <io.h>
#include <windows.h>
#define GPUTK_USE_WINDOWS
#else /* _MSC_VER */
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define GPUTK_USE_UNIX
#ifdef __APPLE__
#include <mach/mach_time.h>
#define GPUTK_USE_DARWIN
#else /* __APPLE__ */
#define GPUTK_USE_LINUX
#endif /* __APPLE__ */
#endif /* _MSC_VER */

#define gpuTKStmt(stmt) stmt

/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/

#define gpuTKLine __LINE__
#define gpuTKFile __FILE__
#define gpuTKFunction __func__

#define gpuTKExit()                                                          \
  gpuTKAssert(0);                                                            \
  exit(1)

#ifdef JSON_OUTPUT
#ifdef GPUTK_USE_COURSERA
#define gpuTKLogger_printOnExit 1
#else /* GPUTK_USE_COURSERA */
#define gpuTKLogger_printOnLog 1
#endif /* GPUTK_USE_COURSERA */
#endif /* JSON_OUTPUT */

/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/

#ifdef __cplusplus
#define EXTERN_C extern "C"
#define START_EXTERN_C EXTERN_C {
#define END_EXTERN_C }
#else
#define EXTERN_C
#define START_EXTERN_C
#define END_EXTERN_C
#endif /* __cplusplus */

/***********************************************************/
/***********************************************************/
/***********************************************************/

#ifndef GPUTK_USE_JSON11
#define GPUTK_USE_JSON11 1
#endif /* GPUTK_USE_JSON11 */

#if 1 || GPUTK_USE_JSON11
#include "vendor/json11.hpp"
#endif /* GPUTK_USE_JSON11 */

/***********************************************************/
/***********************************************************/
/***********************************************************/

#define LAZY_FILE_LOAD
extern char *solutionJSON;

/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/

#ifdef GPUTK_USE_OPENCL
#ifdef GPUTK_USE_DARWIN
#include <OpenCL/opencl.h>
#else /* GPUTK_USE_DARWIN */
#include <CL/cl.h>
#endif /* GPUTK_USE_DARWIN */
#endif /* GPUTK_USE_OPENCL */

#include "gputkTypes.h"

#include "gputkAssert.h"
#include "gputkMalloc.h"
#include "gputkString.h"
#include "gputkUtils.h"

#include "gputkArg.h"
#include "gputkCUDA.h"
#include "gputkCast.h"
#include "gputkComparator.h"
#include "gputkDirectory.h"
#include "gputkExit.h"
#include "gputkExport.h"
#include "gputkFile.h"
#include "gputkImage.h"
#include "gputkImport.h"
#include "gputkInit.h"
#include "gputkLogger.h"
#include "gputkMD5.h"
#include "gputkMPI.h"
#include "gputkSolution.h"
#include "gputkSort.h"
#include "gputkSparse.h"
#include "gputkThrust.h"
#include "gputkTimer.h"
#include "gputkPath.h"

#include "gputkDataset.h"

/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/

#endif /* __GPUTK_H__ */
