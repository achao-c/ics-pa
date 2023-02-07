#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t lgt = 0;
  while ((*s) != '\0') {
    ++lgt;
    ++s;
  }
  return lgt; 
}

char *strcpy(char *dst, const char *src) {
  char* dst1 = dst;
  while ((*src) != '\0') {
    *dst = *src;
    ++src; ++dst;
  }
  (*dst) = '\0';
  return dst1;
}

char *strncpy(char *dst, const char *src, size_t n) {
  if (!n) return dst;
  char * dst1 = dst;
  size_t lgt = strlen(src);
  if (n <= lgt) {
    while (n--) {
      *dst = *src;
      ++src; ++dst;
    } 
    (*dst) = '\0';
    return dst1;
  }
  else {
    while (lgt--) {
      *dst = *src;
      ++src; ++dst;
    }
    size_t end_num = n-lgt;
     while (end_num--) {
      (*dst) = '\0';
      ++dst;
    }
    return dst1;
  }
}

char *strcat(char *dst, const char *src) {
  char* dst1 = dst;
  size_t lgt = strlen(dst);
  dst += lgt;
  strcpy(dst, src);
  return dst1;
}

int strcmp(const char *s1, const char *s2) {
  while (((*s1) != '\0') && ((*s2) != '\0')) {
    if ((*s1) < (*s2)) return -1;
    if ((*s1) > (*s2)) return  1;
    ++s1; ++s2;
  }
  if (((*s1) == '\0') && ((*s2) == '\0')) return 0;
  else if ((*s1) == '\0') return -1;
  return 1;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  if (!n) return 0;
  while (((*s1) != '\0') && ((*s2) != '\0') && (n--)) {
    if ((*s1) < (*s2)) return -1;
    if ((*s1) > (*s2)) return  1;
    ++s1; ++s2;
  }
  if (!n) return 0;
  if (((*s1) == '\0') && ((*s2) == '\0')) return 0;
  else if ((*s1) == '\0') return -1;
  return 1;
}

void *memset(void *s, int c, size_t n) {
  if (!n) return s;
  char* s1 = (char*) s;
  while (n--) {
    (*s1) = (char)c;
    ++s1;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  char* in1 = (char*) in;
  char* out1 = (char*) out;
  while (n--) {
    *out1 = *in1;
    ++in1; ++out1;
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  char* s3 = (char*) s1;
  char* s4 = (char*) s2;
  while (n--) {
    if((*s3) < (*s4)) return -1;
    if((*s3) > (*s4)) return 1;
    ++s3; ++s4;
  }
  return 0;
}

#endif
