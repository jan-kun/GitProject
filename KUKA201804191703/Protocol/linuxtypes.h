#ifndef LINUX_TYPE
#define LINUX_TYPE


typedef signed char      i8;
typedef unsigned char    u8;
typedef signed short    i16;
typedef unsigned short  u16;
typedef signed long     i32;
typedef unsigned long   u32;

typedef volatile u8      v8;   /*  8-bit register                      */
typedef volatile u16    v16;   /* 16-bit register                      */
typedef volatile u32    v32;   /* 32-bit register                      */

#define DPR  57.295779513082
#define RPD  0.017453292519943
#define PI   3.1415926535898

#endif  /*  LINUXTYPE */
