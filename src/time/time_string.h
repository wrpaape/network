#ifndef NETWORK_TIME_TIME_STRING_H_
#define NETWORK_TIME_TIME_STRING_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "time/time_utils.h"		/* Timestamp, time utils */
#include "string/string_utils.h"	/* string utils */

/* macro constants
 *─────────────────────────────────────────────────────────────────────────── */
#define SIZE_TIMESTAMP_STRING	20lu
#define LENGTH_TIMESTAMP_STRING 19lu

#define SIZE_DATETIME_STRING	20lu
#define LENGTH_DATETIME_STRING	19lu

/* struct declarations, typedefs
 *─────────────────────────────────────────────────────────────────────────── */
struct TimestampBuffer {
	char bytes[SIZE_TIMESTAMP_STRING];
};

struct DatetimeBuffer {
	char bytes[SIZE_DATETIME_STRING];
};

/* function-like macros
 *─────────────────────────────────────────────────────────────────────────── */
#define PUT_TIMESTAMP_STRING(PTR, STRING)				\
({*((struct TimestampBuffer *restrict) PTR)				\
= *((const struct TimestampBuffer *const restrict) STRING);		\
PTR = (__typeof__(PTR)) (((struct TimestampBuffer *restrict) PTR) + 1l);})

#define SET_TIMESTAMP_STRING(PTR, STRING)				\
  *((struct TimestampBuffer *restrict) PTR)				\
= *((const struct TimestampBuffer *const restrict) STRING)

#define PUT_DATETIME_STRING(PTR, STRING)				\
({*((struct DatetimeBuffer *restrict) PTR)				\
= *((const struct DatetimeBuffer *const restrict) STRING);		\
PTR = (__typeof__(PTR)) (((struct DatetimeBuffer *restrict) PTR) + 1l);})

/* global variables
 *─────────────────────────────────────────────────────────────────────────── */
extern const char *weekday_map[7];
extern const char *month_map[13];
extern const char *double_digit_map[60];


inline char *
put_year(char *restrict string,
	 unsigned int year)
{
	*string = (char) DIGIT_TO_ASCII(year / 1000u);
	year %= 1000u;

	++string;

	*string = (char) DIGIT_TO_ASCII(year / 100u);
	year %= 100u;

	++string;

	*string = (char) DIGIT_TO_ASCII(year / 10u);
	year %= 10u;

	++string;

	*string = (char) DIGIT_TO_ASCII(year);
	return string + 1;
}

inline char *
put_time(char *restrict string,
	 const unsigned int hour,
	 const unsigned int minute,
	 const unsigned int second)
{
	/* "HH:" */
	PUT_STRING_WIDTH(string,
			 double_digit_map[hour],
			 2);

	PUT_CHAR(string, ':');

	/* "MM:" */
	PUT_STRING_WIDTH(string,
			 double_digit_map[minute],
			 2);

	PUT_CHAR(string, ':');

	/* "SS" */
	PUT_STRING_WIDTH(string,
			 double_digit_map[second],
			 2);
	return string;
}

inline void
timestamp_string_init(char *restrict string,
		      const struct Timestamp *const restrict timestamp)
{
	string = put_year(string,
			  timestamp->year);

	*string = '-';
	++string;

	PUT_STRING_WIDTH(string,
			 double_digit_map[timestamp->month],
			 2);

	*string = '-';
	++string;

	/* "DD " */
	PUT_STRING_WIDTH(string,
			 double_digit_map[timestamp->day],
			 2);

	*string = ' ';
	++string;


	string = put_time(string,
			  timestamp->hour,
			  timestamp->minute,
			  timestamp->second);

	*string = '\0';
}

#define datetime_string_init(STRING, TIMESTAMP)				\
timestamp_string_init(STRING, TIMESTAMP)

inline void
timestamp_string_increment_overflow(char *const restrict last)
{
	/* "YYYY-MM-DD HH:MM:S8" */
	/* min:	1970-01-01 00:00:01 */
	/* max: 2038-01-19 03:14:07 */

	char *restrict ptr = last - 1l;

	if (*ptr != '0') {
		*last = '8';
		return;
	}

	ptr -= 2l;

	if (*ptr != '4') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '1') {
		*last = '8';
		return;
	}

	ptr -= 2l;

	if (*ptr != '3') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '0') {
		*last = '8';
		return;
	}

	ptr -= 2l;

	if (*ptr != '9') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '1') {
		*last = '8';
		return;
	}

	ptr -= 2l;

	if (*ptr != '1') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '0') {
		*last = '8';
		return;
	}

	ptr -= 2l;

	if (*ptr != '8') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '3') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '0') {
		*last = '8';
		return;
	}

	--ptr;

	if (*ptr != '2') {
		*last = '8';
		return;
	}

	SET_STRING_WIDTH(ptr, "1970", 4);

	ptr += 8l;

	SET_STRING_WIDTH(ptr, "01 00:00:01", 11);
}

inline void
timestamp_string_increment(char *restrict ptr)
{
	/* "YYYY-MM-DD HH:MM:Sx" */
	switch (*ptr) {
	case '0': *ptr = '1'; return;
	case '1': *ptr = '2'; return;
	case '2': *ptr = '3'; return;
	case '3': *ptr = '4'; return;
	case '4': *ptr = '5'; return;
	case '5': *ptr = '6'; return;
	case '6': *ptr = '7'; return;
	case '7': timestamp_string_increment_overflow(ptr); return;
	case '8': *ptr = '9'; return;
	default: --ptr;
	}

	/* "YYYY-MM-DD HH:MM:x0" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "10", 2); return;
	case '1': SET_STRING_WIDTH(ptr, "20", 2); return;
	case '2': SET_STRING_WIDTH(ptr, "30", 2); return;
	case '3': SET_STRING_WIDTH(ptr, "40", 2); return;
	case '4': SET_STRING_WIDTH(ptr, "50", 2); return;
	default:
		ptr -= 2l;
	}

	/* "YYYY-MM-DD HH:Mx:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1:00", 4); return;
	case '1': SET_STRING_WIDTH(ptr, "2:00", 4); return;
	case '2': SET_STRING_WIDTH(ptr, "3:00", 4); return;
	case '3': SET_STRING_WIDTH(ptr, "4:00", 4); return;
	case '4': SET_STRING_WIDTH(ptr, "5:00", 4); return;
	case '5': SET_STRING_WIDTH(ptr, "6:00", 4); return;
	case '6': SET_STRING_WIDTH(ptr, "7:00", 4); return;
	case '7': SET_STRING_WIDTH(ptr, "8:00", 4); return;
	case '8': SET_STRING_WIDTH(ptr, "9:00", 4); return;
	default:
		--ptr;
	}

	/* "YYYY-MM-DD HH:x0:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "10:00", 5); return;
	case '1': SET_STRING_WIDTH(ptr, "20:00", 5); return;
	case '2': SET_STRING_WIDTH(ptr, "30:00", 5); return;
	case '3': SET_STRING_WIDTH(ptr, "40:00", 5); return;
	case '4': SET_STRING_WIDTH(ptr, "50:00", 5); return;
	default:
		ptr -= 2l;
	}

	/* "YYYY-MM-DD Hx:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1:00:00", 7); return;
	case '1': SET_STRING_WIDTH(ptr, "2:00:00", 7); return;
	case '2': SET_STRING_WIDTH(ptr, "3:00:00", 7); return;
	case '3':
		if (ptr[-1] == '2') {
			SET_STRING_WIDTH(&ptr[-1], "00:00:00", 8);
			ptr -= 3l;
			break;
		}
		  SET_STRING_WIDTH(ptr, "4:00:00", 7); return;
	case '4': SET_STRING_WIDTH(ptr, "5:00:00", 7); return;
	case '5': SET_STRING_WIDTH(ptr, "6:00:00", 7); return;
	case '6': SET_STRING_WIDTH(ptr, "7:00:00", 7); return;
	case '7': SET_STRING_WIDTH(ptr, "8:00:00", 7); return;
	case '8': SET_STRING_WIDTH(ptr, "9:00:00", 7); return;
	default:
		--ptr;

		if (*ptr == '0')
			SET_STRING_WIDTH(ptr, "10:00:00", 8);
		else
			SET_STRING_WIDTH(ptr, "20:00:00", 8);

		return;
	}

	/* "YYYY-MM-Dx 00:00:00" */
	switch (*ptr) {
	default: /* 'X0' and ill-formed days, guaranteed to set valid, return */
		if (ptr[-1] != '3') {
			*ptr = '1';

		} else if (ptr[-4] == '1') {
			if (ptr[-3] == '2')
				*ptr = '1';
			else
				SET_STRING_WIDTH(&ptr[-3],  "2-01", 4);

		} else {
			switch (ptr[-3]) {
			case '4':
				SET_STRING_WIDTH(&ptr[-3],  "5-01", 4);
				return;
			case '6':
				SET_STRING_WIDTH(&ptr[-3],  "7-01", 4);
				return;
			case '9':
				SET_STRING_WIDTH(&ptr[-4], "10-01", 5);
				return;
			default:
				*ptr = '1';
			}
		}
		return;


	case '1':
		if (ptr[-1] != '3') {
			*ptr = '2';
			return;

		} else if (ptr[-4] == '1') {	/* new year */
			ptr -= 6l;
			break;
		}

		ptr -= 3l;

		switch (*ptr) {
		case '0': SET_STRING_WIDTH(ptr, "1-01", 4); return;
		case '1': SET_STRING_WIDTH(ptr, "2-01", 4); return;
		case '3': SET_STRING_WIDTH(ptr, "4-01", 4); return;
		case '5': SET_STRING_WIDTH(ptr, "6-01", 4); return;
		case '7': SET_STRING_WIDTH(ptr, "8-01", 4); return;
		default:  SET_STRING_WIDTH(ptr, "9-01", 4); return;
		}


	case '2': *ptr = '3'; return;
	case '3': *ptr = '4'; return;
	case '4': *ptr = '5'; return;
	case '5': *ptr = '6'; return;
	case '6': *ptr = '7'; return;
	case '7': *ptr = '8'; return;


	case '8':
		if ((ptr[-4] == '1') || (ptr[-3] != '2') || (ptr[-1] != '2')) {
			++(ptr[-1]);
			*ptr = '0';

		} else {	/* 02-28 → check if leap year */

			const char *restrict year_ptr = ptr - 6l;
			unsigned int year = ASCII_TO_DIGIT(*year_ptr);
			--year_ptr;

			year += ASCII_TO_DIGIT(*year_ptr) * 10u;
			--year_ptr;

			year += ASCII_TO_DIGIT(*year_ptr) * 100u;
			--year_ptr;

			year += ASCII_TO_DIGIT(*year_ptr) * 1000u;

			if (is_leap_year(year))
				*ptr = '9';
			else
				SET_STRING_WIDTH(&ptr[-3], "3-01", 4);
		}
		return;


	case '9':
		if ((ptr[-4] == '1') || (ptr[-3] != '2') || (ptr[-1] != '2')) {
			++(ptr[-1]);
			*ptr = '0';

		} else {	/* 02-29 → 03-01 */
			SET_STRING_WIDTH(&ptr[-4], "03-01", 5);
		}
		return;
	}

	/* "YYYx-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1-01-01", 7); return;
	case '1': SET_STRING_WIDTH(ptr, "2-01-01", 7); return;
	case '2': SET_STRING_WIDTH(ptr, "3-01-01", 7); return;
	case '3': SET_STRING_WIDTH(ptr, "4-01-01", 7); return;
	case '4': SET_STRING_WIDTH(ptr, "5-01-01", 7); return;
	case '5': SET_STRING_WIDTH(ptr, "6-01-01", 7); return;
	case '6': SET_STRING_WIDTH(ptr, "7-01-01", 7); return;
	case '7': SET_STRING_WIDTH(ptr, "8-01-01", 7); return;
	case '8': SET_STRING_WIDTH(ptr, "9-01-01", 7); return;
	default:
		--ptr;
	}

	/* "YYx0-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "10-01-01", 8); return;
	case '1': SET_STRING_WIDTH(ptr, "20-01-01", 8); return;
	case '2': SET_STRING_WIDTH(ptr, "30-01-01", 8); return;
	case '3': SET_STRING_WIDTH(ptr, "40-01-01", 8); return;
	case '4': SET_STRING_WIDTH(ptr, "50-01-01", 8); return;
	case '5': SET_STRING_WIDTH(ptr, "60-01-01", 8); return;
	case '6': SET_STRING_WIDTH(ptr, "70-01-01", 8); return;
	case '7': SET_STRING_WIDTH(ptr, "80-01-01", 8); return;
	case '8': SET_STRING_WIDTH(ptr, "90-01-01", 8); return;
	default:
		--ptr;
	}

	/* "Yx00-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "100-01-01", 9); return;
	case '1': SET_STRING_WIDTH(ptr, "200-01-01", 9); return;
	case '2': SET_STRING_WIDTH(ptr, "300-01-01", 9); return;
	case '3': SET_STRING_WIDTH(ptr, "400-01-01", 9); return;
	case '4': SET_STRING_WIDTH(ptr, "500-01-01", 9); return;
	case '5': SET_STRING_WIDTH(ptr, "600-01-01", 9); return;
	case '6': SET_STRING_WIDTH(ptr, "700-01-01", 9); return;
	case '7': SET_STRING_WIDTH(ptr, "800-01-01", 9); return;
	case '8': SET_STRING_WIDTH(ptr, "900-01-01", 9); return;
	default: /* must be Y2K if valid MySQL TIMESTAMP */
		  SET_STRING_WIDTH(&ptr[-1], "2000-01-01", 10);
	}
}


inline void
datetime_string_increment(char *restrict ptr)
{
	/* "YYYY-MM-DD HH:MM:Sx" */
	switch (*ptr) {
	case '0': *ptr = '1'; return;
	case '1': *ptr = '2'; return;
	case '2': *ptr = '3'; return;
	case '3': *ptr = '4'; return;
	case '4': *ptr = '5'; return;
	case '5': *ptr = '6'; return;
	case '6': *ptr = '7'; return;
	case '7': *ptr = '8'; return;
	case '8': *ptr = '9'; return;
	default: --ptr;
	}

	/* "YYYY-MM-DD HH:MM:x0" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "10", 2); return;
	case '1': SET_STRING_WIDTH(ptr, "20", 2); return;
	case '2': SET_STRING_WIDTH(ptr, "30", 2); return;
	case '3': SET_STRING_WIDTH(ptr, "40", 2); return;
	case '4': SET_STRING_WIDTH(ptr, "50", 2); return;
	default:
		ptr -= 2l;
	}

	/* "YYYY-MM-DD HH:Mx:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1:00", 4); return;
	case '1': SET_STRING_WIDTH(ptr, "2:00", 4); return;
	case '2': SET_STRING_WIDTH(ptr, "3:00", 4); return;
	case '3': SET_STRING_WIDTH(ptr, "4:00", 4); return;
	case '4': SET_STRING_WIDTH(ptr, "5:00", 4); return;
	case '5': SET_STRING_WIDTH(ptr, "6:00", 4); return;
	case '6': SET_STRING_WIDTH(ptr, "7:00", 4); return;
	case '7': SET_STRING_WIDTH(ptr, "8:00", 4); return;
	case '8': SET_STRING_WIDTH(ptr, "9:00", 4); return;
	default:
		--ptr;
	}

	/* "YYYY-MM-DD HH:x0:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "10:00", 5); return;
	case '1': SET_STRING_WIDTH(ptr, "20:00", 5); return;
	case '2': SET_STRING_WIDTH(ptr, "30:00", 5); return;
	case '3': SET_STRING_WIDTH(ptr, "40:00", 5); return;
	case '4': SET_STRING_WIDTH(ptr, "50:00", 5); return;
	default:
		ptr -= 2l;
	}

	/* "YYYY-MM-DD Hx:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1:00:00", 7); return;
	case '1': SET_STRING_WIDTH(ptr, "2:00:00", 7); return;
	case '2': SET_STRING_WIDTH(ptr, "3:00:00", 7); return;
	case '3':
		if (ptr[-1] == '2') {
			SET_STRING_WIDTH(&ptr[-1], "00:00:00", 8);
			ptr -= 3l;
			break;
		}
		  SET_STRING_WIDTH(ptr, "4:00:00", 7); return;
	case '4': SET_STRING_WIDTH(ptr, "5:00:00", 7); return;
	case '5': SET_STRING_WIDTH(ptr, "6:00:00", 7); return;
	case '6': SET_STRING_WIDTH(ptr, "7:00:00", 7); return;
	case '7': SET_STRING_WIDTH(ptr, "8:00:00", 7); return;
	case '8': SET_STRING_WIDTH(ptr, "9:00:00", 7); return;
	default:
		--ptr;

		if (*ptr == '0')
			SET_STRING_WIDTH(ptr, "10:00:00", 8);
		else
			SET_STRING_WIDTH(ptr, "20:00:00", 8);

		return;
	}

	/* "YYYY-MM-Dx 00:00:00" */
	switch (*ptr) {
	default: /* 'X0' and ill-formed days, guaranteed to set valid, return */
		if (ptr[-1] != '3') {
			*ptr = '1';

		} else if (ptr[-4] == '1') {
			if (ptr[-3] == '2')
				*ptr = '1';
			else
				SET_STRING_WIDTH(&ptr[-3],  "2-01", 4);

		} else {
			switch (ptr[-3]) {
			case '4':
				SET_STRING_WIDTH(&ptr[-3],  "5-01", 4);
				return;
			case '6':
				SET_STRING_WIDTH(&ptr[-3],  "7-01", 4);
				return;
			case '9':
				SET_STRING_WIDTH(&ptr[-4], "10-01", 5);
				return;
			default:
				*ptr = '1';
			}
		}
		return;


	case '1':
		if (ptr[-1] != '3') {
			*ptr = '2';
			return;

		} else if (ptr[-4] == '1') {	/* new year */
			ptr -= 6l;
			break;
		}

		ptr -= 3l;

		switch (*ptr) {
		case '0': SET_STRING_WIDTH(ptr, "1-01", 4); return;
		case '1': SET_STRING_WIDTH(ptr, "2-01", 4); return;
		case '3': SET_STRING_WIDTH(ptr, "4-01", 4); return;
		case '5': SET_STRING_WIDTH(ptr, "6-01", 4); return;
		case '7': SET_STRING_WIDTH(ptr, "8-01", 4); return;
		default:  SET_STRING_WIDTH(ptr, "9-01", 4); return;
		}


	case '2': *ptr = '3'; return;
	case '3': *ptr = '4'; return;
	case '4': *ptr = '5'; return;
	case '5': *ptr = '6'; return;
	case '6': *ptr = '7'; return;
	case '7': *ptr = '8'; return;


	case '8':
		if ((ptr[-4] == '1') || (ptr[-3] != '2') || (ptr[-1] != '2')) {
			++(ptr[-1]);
			*ptr = '0';

		} else {	/* 02-28 → check if leap year */

			const char *restrict year_ptr = ptr - 6l;
			unsigned int year = ASCII_TO_DIGIT(*year_ptr);
			--year_ptr;

			year += ASCII_TO_DIGIT(*year_ptr) * 10u;
			--year_ptr;

			year += ASCII_TO_DIGIT(*year_ptr) * 100u;
			--year_ptr;

			year += ASCII_TO_DIGIT(*year_ptr) * 1000u;

			if (is_leap_year(year))
				*ptr = '9';
			else
				SET_STRING_WIDTH(&ptr[-3], "3-01", 4);
		}
		return;


	case '9':
		if ((ptr[-4] == '1') || (ptr[-3] != '2') || (ptr[-1] != '2')) {
			++(ptr[-1]);
			*ptr = '0';

		} else {	/* 02-29 → 03-01 */
			SET_STRING_WIDTH(&ptr[-4], "03-01", 5);
		}
		return;
	}

	/* "YYYx-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1-01-01", 7); return;
	case '1': SET_STRING_WIDTH(ptr, "2-01-01", 7); return;
	case '2': SET_STRING_WIDTH(ptr, "3-01-01", 7); return;
	case '3': SET_STRING_WIDTH(ptr, "4-01-01", 7); return;
	case '4': SET_STRING_WIDTH(ptr, "5-01-01", 7); return;
	case '5': SET_STRING_WIDTH(ptr, "6-01-01", 7); return;
	case '6': SET_STRING_WIDTH(ptr, "7-01-01", 7); return;
	case '7': SET_STRING_WIDTH(ptr, "8-01-01", 7); return;
	case '8': SET_STRING_WIDTH(ptr, "9-01-01", 7); return;
	default:
		--ptr;
	}

	/* "YYx0-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "10-01-01", 8); return;
	case '1': SET_STRING_WIDTH(ptr, "20-01-01", 8); return;
	case '2': SET_STRING_WIDTH(ptr, "30-01-01", 8); return;
	case '3': SET_STRING_WIDTH(ptr, "40-01-01", 8); return;
	case '4': SET_STRING_WIDTH(ptr, "50-01-01", 8); return;
	case '5': SET_STRING_WIDTH(ptr, "60-01-01", 8); return;
	case '6': SET_STRING_WIDTH(ptr, "70-01-01", 8); return;
	case '7': SET_STRING_WIDTH(ptr, "80-01-01", 8); return;
	case '8': SET_STRING_WIDTH(ptr, "90-01-01", 8); return;
	default:
		--ptr;
	}

	/* "Yx00-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "100-01-01", 9); return;
	case '1': SET_STRING_WIDTH(ptr, "200-01-01", 9); return;
	case '2': SET_STRING_WIDTH(ptr, "300-01-01", 9); return;
	case '3': SET_STRING_WIDTH(ptr, "400-01-01", 9); return;
	case '4': SET_STRING_WIDTH(ptr, "500-01-01", 9); return;
	case '5': SET_STRING_WIDTH(ptr, "600-01-01", 9); return;
	case '6': SET_STRING_WIDTH(ptr, "700-01-01", 9); return;
	case '7': SET_STRING_WIDTH(ptr, "800-01-01", 9); return;
	case '8': SET_STRING_WIDTH(ptr, "900-01-01", 9); return;
	default:
		--ptr;
	}

	/* "x000-01-01 00:00:00" */
	switch (*ptr) {
	case '0': SET_STRING_WIDTH(ptr, "1000-01-01", 10); return;
	case '1': SET_STRING_WIDTH(ptr, "2000-01-01", 10); return;
	case '2': SET_STRING_WIDTH(ptr, "3000-01-01", 10); return;
	case '3': SET_STRING_WIDTH(ptr, "4000-01-01", 10); return;
	case '4': SET_STRING_WIDTH(ptr, "5000-01-01", 10); return;
	case '5': SET_STRING_WIDTH(ptr, "6000-01-01", 10); return;
	case '6': SET_STRING_WIDTH(ptr, "7000-01-01", 10); return;
	case '7': SET_STRING_WIDTH(ptr, "8000-01-01", 10); return;
	case '8': SET_STRING_WIDTH(ptr, "9000-01-01", 10); return;
	default:  SET_STRING_WIDTH(ptr, "1000-01-01", 10); /* MySQL min date */
	}
}


inline const char *
weekday_string(const unsigned int year,
	       const unsigned int month,
	       const unsigned int day)
{
	/* since March 1, 4801 BC */
	const unsigned int alpha        = (14 - month) / 12;
	const unsigned int months_since = month + (12 * alpha) - 3;
	const unsigned int years_since  = year + 4800 - alpha;

	const unsigned int julian_day_number = day
					     + (((153 * months_since) + 2) / 5)
					     + (365 * years_since)
					     + (years_since / 4)
					     - (years_since / 100)
					     + (years_since / 400)
					     - 32045;

	return weekday_map[julian_day_number % 7];
}


inline char *
put_http_date(char *restrict string,
	      const struct Timestamp *const restrict timestamp)
{
	PUT_STRING_WIDTH(string,
			 weekday_string(timestamp->year,
					timestamp->month,
					timestamp->day),
			 3);

	PUT_STRING_WIDTH(string, ", ", 2);

	PUT_STRING_WIDTH(string,
			 double_digit_map[timestamp->day],
			 2);

	PUT_CHAR(string, ' ');

	PUT_STRING_WIDTH(string,
			 month_map[timestamp->month],
			 3);

	PUT_CHAR(string, ' ');

	string = put_year(string,
			  timestamp->year);

	PUT_CHAR(string, ' ');

	string = put_time(string,
			  timestamp->hour,
			  timestamp->minute,
			  timestamp->second);

	PUT_STRING_WIDTH(string, " GMT", 4);

	return string;
}

inline char *
put_http_time_element(char *restrict string,
		      const struct Timestamp *const restrict timestamp)
{
	PUT_STRING_WIDTH(string, "<time datetime=\"", 16);

	string = put_year(string,
			  timestamp->year);

	PUT_CHAR(string, '-');

	PUT_STRING_WIDTH(string,
			 double_digit_map[timestamp->month],
			 2);

	PUT_CHAR(string, '-');

	char *const restrict day = string;

	PUT_STRING_WIDTH(string,
			 double_digit_map[timestamp->day],
			 2);

	PUT_CHAR(string, 'T');

	char *const restrict time = string;

	string = put_time(string,
			  timestamp->hour,
			  timestamp->minute,
			  timestamp->second);

	PUT_STRING_WIDTH(string, "\">", 2);

	string = put_string(string,
			    weekday_string(timestamp->year,
					   timestamp->month,
					   timestamp->day));

	PUT_STRING_WIDTH(string, ", ", 2);

	string = put_string(string,
			    month_map[timestamp->month]);

	PUT_CHAR(string, ' ');

	PUT_STRING_WIDTH(string, day, 2);

	PUT_STRING_WIDTH(string, ", ", 2);

	PUT_STRING_WIDTH(string, time, 8);

	PUT_STRING_WIDTH(string, "</time>", 7);

	return string;
}

#endif /* ifndef NETWORK_TIME_TIME_STRING_H_ */
