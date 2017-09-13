#ifndef _ENGINE_DEF_ENVIRONMENT_DEF_H_
#define _ENGINE_DEF_ENVIRONMENT_DEF_H_

//--------------windows----
#if defined(WIN)
#define CSG_WIN
#undef CSG_LINUX
#endif

//abort endian
#if defined(CSG_WIN) || defined(__i386) || defined(__x86_64)
#define CSG_LITTLE_ENDIAN   1
#endif


//--------------linux------

#ifdef __linux
#define CSG_LINUX
#undef CSG_WIN
#endif


#endif