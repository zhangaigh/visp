/****************************************************************************
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2015 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://visp.inria.fr for more information.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, please contact
 * Inria at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * Time management and measurement.
 *
 * Authors:
 * Eric Marchand
 * Fabien Spindler
 *
 *****************************************************************************/


#include <ctime>

#include <visp3/core/vpTime.h>
#include <visp3/core/vpDebug.h>


/*!
  \file vpTime.cpp
  \brief Time management and measurement

*/


// Unix depend version

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
#  include <sys/time.h>
#  include <unistd.h>
#elif defined(_WIN32)
#  include <windows.h>
#  include <winbase.h>
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace vpTime
{
#endif

/*!
   This time is in milli-seconds. Threshold to activate usleep() in waiting
   methods (see wait()). This threshold is needed, because usleep() is not
   accurate on many machines.  Call sleep() functions during the time to wait
   minus vpTime::minTimeForUsleepCall. The rest of the time to wait is managed
   by a while loop.
*/
double minTimeForUsleepCall = 4;

/*!
   \return The time during which a while loop is used to handle the time
   wainting functions. When the time to wait is greater than that value, we use
   non blocking functions like usleep() instead.
 */
double getMinTimeForUsleepCall()
{
  return minTimeForUsleepCall;
}

/*!
  \fn vpTime::measureTimeMs()
  Return the time in milliseconds since January 1st 1970.

  \sa measureTimeMicros(), measureTimeSecond()
*/
double measureTimeMs()
{
#if defined(_WIN32)
  LARGE_INTEGER time, frequency;
  QueryPerformanceFrequency(&frequency);
  if(frequency.QuadPart == 0){
    return(timeGetTime());
  }
  else{
    QueryPerformanceCounter(&time);
    return (double)(1000.0*time.QuadPart/frequency.QuadPart);
  }
#elif !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
  struct timeval tp;
  gettimeofday(&tp,0);
  return(1000.0*tp.tv_sec + tp.tv_usec/1000.0);
#endif
}

/*!
  Return the time in microseconds since January 1st 1970.

  \sa measureTimeMs(), measureTimeSecond()
*/
double measureTimeMicros()
{
#if defined(_WIN32)
  LARGE_INTEGER time, frequency;
  QueryPerformanceFrequency(&frequency);
  if(frequency.QuadPart == 0){
    return(timeGetTime());
  }
  else{
    QueryPerformanceCounter(&time);
    return (double)(1000000.0*time.QuadPart/frequency.QuadPart);
  }
#else

  struct timeval tp;
  gettimeofday(&tp,0);
  return(1000000.0*tp.tv_sec + tp.tv_usec);
#endif
}

/*!

  Wait t miliseconds after t0 (in ms).

  The waiting is done by a call to usleep() if the time to wait is greater than
  vpTime::minTimeForUsleepCall.

  \param t0 : reference time (in ms)
  \param t  : time to wait (in ms)

  \return 0 : The function did wait.
  \return 1 : The time was already over, no need to wait.
*/
int wait(double t0, double t)
{
  double timeCurrent, timeToWait;
  timeCurrent = measureTimeMs();

  timeToWait = t0 + t - timeCurrent;

  if ( timeToWait <= 0. ) // no need to wait
    return(1);
  else {
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
    if (timeToWait > vpTime::minTimeForUsleepCall) {
      usleep((useconds_t)((timeToWait-vpTime::minTimeForUsleepCall)*1000));
    }
#elif defined(_WIN32)
    if (timeToWait > vpTime::minTimeForUsleepCall) {
      Sleep((DWORD)(timeToWait-vpTime::minTimeForUsleepCall));
    }
#endif
    // Blocking loop to have an accurate waiting
    do {
      timeCurrent = measureTimeMs();
      timeToWait = t0 + t - timeCurrent;

    } while (timeToWait > 0.);

    return 0;
  }
}

/*!
  Wait t miliseconds from now.

  The waiting is done by a call to usleep() if the time to wait is greater than
  vpTime::minTimeForUsleepCall.

  \param t : Time to wait in ms.

*/
void wait(double t)
{
  double t0, timeCurrent, timeToWait;
  t0 = timeCurrent = measureTimeMs();

  timeToWait = t;

  if ( timeToWait <= 0. ) // no need to wait
    return;
  else {
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
    if (timeToWait > vpTime::minTimeForUsleepCall) {
      usleep((useconds_t)((timeToWait-vpTime::minTimeForUsleepCall)*1000));
    }
#elif defined(_WIN32)
    if (timeToWait > vpTime::minTimeForUsleepCall) {
      Sleep((DWORD)(timeToWait-vpTime::minTimeForUsleepCall));
    }
#endif
    // Blocking loop to have an accurate waiting
    do {
      timeCurrent = measureTimeMs();
      timeToWait = t0 + t - timeCurrent;

    } while (timeToWait > 0.);

    return;
  }
}

/*!

  Return the measured time in seconds since January 1st 1970.

  \sa measureTimeMs()
*/
double  measureTimeSecond()
{
  return vpTime::measureTimeMs()/1000.0 ;
}

/*!
  Sleep t miliseconds from now.

  \param t : Time to sleep in ms.

*/
void sleepMs(double t)
{
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
  usleep((useconds_t)(t*1000));
#elif defined(_WIN32)
  Sleep((DWORD)(t));
#endif
}

/*!
   Return a string containing date and time.

   \param[in] format : The string format supported by strftime() function that represents the time.
   The default format is the following \c "%Y/%m/%d %H:%M:%S".
   This string contains any combination of special format specifiers given in the next table:
   | specifier | Replaced by                                          | Example                  |
   |-----------|------------------------------------------------------|--------------------------|
   | %%a	     | Abbreviated weekday name *                           | Thu                      |
   | %%A	     | Full weekday name *                                  | Thursday                 |
   | %%b	     | Abbreviated month name *                             | Aug                      |
   | %%B	     | Full month name *                                    | August                   |
   | %%c	     | Date and time representation *                       | Thu Aug 23 14:55:02 2001 |
   | %%C	     | Year divided by 100 and truncated to integer (00-99) | 20                       |
   | %%d	     | Day of the month, zero-padded (01-31)                | 23                       |
   | %%D	     | Short MM/DD/YY date, equivalent to %m/%d/%y          | 08/23/01                 |
   | %%e	     | Day of the month, space-padded ( 1-31)               | 23                       |
   | %%F	     | Short YYYY-MM-DD date, equivalent to %Y-%m-%d        | 2001-08-23               |
   | %%g	     | Week-based year, last two digits (00-99)             | 01                       |
   | %%G	     | Week-based year                                      | 2001                     |
   | %%h	     | Abbreviated month name * (same as %b)                | Aug                      |
   | %%H	     | Hour in 24h format (00-23)                           | 14                       |
   | %%I	     | Hour in 12h format (01-12)                           | 02                       |
   | %%j	     | Day of the year (001-366)                            | 235                      |
   | %%m	     | Month as a decimal number (01-12)                    | 08                       |
   | %%M	     | Minute (00-59)                                       | 55                       |
   | %%n       | New-line character ('\\n')	                          |                          |
   | %%p       | AM or PM designation	                                | PM                       |
   | %%r       | 12-hour clock time *	                                | 02:55:02 pm              |
   | %%R       | 24-hour HH:MM time, equivalent to %H:%M              | 14:55                    |
   | %%S       | Second (00-61)                                       | 02                       |
   | %%t       | Horizontal-tab character ('\\t')                     |                          |
   | %%T       | ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S | 14:55:02              |
   | %%u       | ISO 8601 weekday as number with Monday as 1 (1-7)    | 4                        |
   | %%U       | Week number with the first Sunday as the first day of week one (00-53) | 33     |
   | %%V       | ISO 8601 week number (00-53)                         | 34                       |
   | %%w       | Weekday as a decimal number with Sunday as 0 (0-6)   | 4                        |
   | %%W       | Week number with the first Monday as the first day of week one (00-53) | 34     |
   | %%x       | Date representation *                                | 08/23/01                 |
   | %%X       | Time representation *                                | 14:55:02                 |
   | %%y       | Year, last two digits (00-99)                        | 01                       |
   | %%Y       | Year                                                 | 2001                     |
   | %%z       | ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100) \n If timezone cannot be determined, no characters	| +100 |
   | %%Z       | Timezone name or abbreviation * \n If timezone cannot be determined, no characters| CDT |
   | %%        | A % sign                                             |	%                        |
   * The specifiers marked with an asterisk (*) are locale-dependent.

   \return A formated date and time string. When default format is used, the returned string contains "YYYY/MM/DD hh:mm:ss".

   The following example shows how to use this function:
   \code
#include <visp3/core/vpTime.h>

int main()
{
  std::cout << "%Y/%m/%d %H:%M:%S (default): " << vpTime::getDateTime() << std::endl;
  std::cout << "%Y-%m-%d_%H.%M.%S format   : " << vpTime::getDateTime("%Y-%m-%d_%H.%M.%S") << std::endl;
  std::cout << "%F                format   : " << vpTime::getDateTime("%F") << std::endl;
  std::cout << "%X                format   : " << vpTime::getDateTime("%X") << std::endl;

  return 0;
}
   \endcode
   It produces the following output:
   \code
%Y/%m/%d %H:%M:%S (default): 2016/10/05 19:42:44
%Y-%m-%d_%H.%M.%S format   : 2016-10-05_19.42.44
%F                format   : 2016-10-05
%X                format   : 19:42:44
   \endcode

 */
std::string getDateTime(const std::string &format) {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, 80, format.c_str(), timeinfo);
  std::string str(buffer);

  return str;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
};
#endif
