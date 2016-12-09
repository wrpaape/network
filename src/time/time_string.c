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

const char *double_digit_map[60] = {
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
};


extern inline char *
put_year(char *restrict string,
	 unsigned int year);

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

extern inline char *
put_http_date(char *restrict string,
	      const struct Timestamp *const restrict timestamp);

extern inline char *
put_http_time_element(char *restrict string,
		      const struct Timestamp *const restrict timestamp);
