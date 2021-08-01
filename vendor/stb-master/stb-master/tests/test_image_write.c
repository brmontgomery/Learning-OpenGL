#ifdef __clang__
#define STBIWDEF static inline
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb-master/stb_image_write.h"