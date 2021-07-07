#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned long getMilliseconds(void);
unsigned long millisecondsElapsedSince(unsigned long millisecondsSince);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* TIME_UTILS_H */