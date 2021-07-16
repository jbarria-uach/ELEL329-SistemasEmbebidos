/**
 * @file timeUtils.h
 * @author Jorge Barría (jebarria@alumnos.uach.cl)
 * @brief Time related utility functions header.
 * @version 0.1
 * @date 2021-07-07
 * 
 * @copyright Copyright (c) 2021 by Universidad Austral de Chile
 * 
 */
#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the milliseconds elapsed since the program beginning.
 * 
 * It's just a wrapper to the Arduino "millis()" function.
 * 
 * @return the number of milliseconds elapsed.
 */
unsigned long getMilliseconds(void);

/**
 * @brief Calculates the number of milliseconds elapsed.
 * 
 * @param millisecondsSince the initial value of milliseconds. Must be less
 *                          than the current milliseconds (as returned by 
 *                          getMilliseconds() function) elapsed value.
 * @return the number of milliseconds elapsed since the "millisecondsSince" 
 *         value.
 */
unsigned long millisecondsElapsedSince(unsigned long millisecondsSince);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* TIME_UTILS_H */