#pragma once

#ifdef _MSC_VER
#define STRCPY(dest,size,src) ::strcpy_s(dest,size,src)
#define STRCAT(dest,size,src) ::strcat_s(dest,size,src)
#define SPRINTF(buf,size,fmt,...) ::sprintf_s(buf,size,fmt,__VA_ARGS__)
#else
#define STRCPY(dest,size,src) ::strcpy(dest,src)
#define STRCAT(dest,size,src) ::strcat(dest,src)
#define SPRINTF(buf,size,fmt,...) ::sprintf(buf,fmt,__VA_ARGS__)
#endif

