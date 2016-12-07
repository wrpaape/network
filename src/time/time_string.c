#include "time/time_string.h"

/* global variables
 *─────────────────────────────────────────────────────────────────────────── */
const char *weekday_map[7] = {
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
	"Sunday"
};

const char *month_map[13] = {
	NULL,
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};


extern inline char *
put_year(char *restrict string,
	 unsigned int year);

extern inline char *
put_month(char *restrict string,
	  unsigned int month);

extern inline char *
put_day(char *restrict string,
	const unsigned int day);

extern inline char *
put_time(char *restrict string,
	 const unsigned int hour,
	 const unsigned int minute,
	 const unsigned int second);

extern inline void
timestamp_string_init(char *restrict string,
		      const struct Timestamp *const restrict timestamp);

extern inline void
timestamp_string_increment_overflow(char *const restrict last);

extern inline void
timestamp_string_increment(char *restrict ptr);

extern inline void
datetime_string_increment(char *restrict ptr);

extern inline const char *
weekday_string(const unsigned int year,
	       const unsigned int month,
	       const unsigned int day);

extern inline const char *
month_string(const unsigned int month);

extern inline char *
put_http_date(char *restrict string,
	      const struct Timestamp *const restrict timestamp);
