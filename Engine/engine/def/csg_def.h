#ifndef _ENGINE_DEF_CSG_DEF_H_
#define _ENGINE_DEF_CSG_DEF_H_

#include <assert.h>
#include <atomic>
#include "engine/def/environment_def.h"

#ifdef CSG_WIN
//to support win32
#include <WinSock2.h>
#include <windows.h>
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef byte_t
typedef unsigned char byte_t;
#endif

#ifndef uint_t
typedef unsigned int uint_t;
#endif

#ifndef ushort_t
typedef unsigned short ushort_t;
#endif

#ifndef size_t
#ifdef CSG_WIN
typedef unsigned int  size_t;
#elif defined CSG_LINUX
typedef unsigned long  size_t;
#define __need_time_t  //for time_t
#endif
#endif

#ifndef long64_t
#ifdef _WIN32
typedef unsigned __int64 ulong64_t;
typedef __int64 long64_t;
#elif defined __x86_64__
typedef long long64_t;
typedef unsigned long ulong64_t;
#else
typedef long long  long64_t;
typedef unsigned long long ulong64_t;
#endif
#endif


#ifndef time_t
typedef long64_t time_t;
#endif

//errno
const int CSG_RETURN_ERROR = -1;
const int CSG_RETURN_OK = 0;


typedef std::atomic<int> csg_atomic_int;
typedef std::atomic<bool> csg_atomic_boolean;

#endif