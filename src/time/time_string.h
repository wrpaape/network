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
put_month(char *restrict string,
	  const unsigned int month)
{
	switch (month) {
	case 12u: PUT_STRING_WIDTH(string, "12", 2); return string;
	case 11u: PUT_STRING_WIDTH(string, "11", 2); return string;
	case 10u: PUT_STRING_WIDTH(string, "10", 2); return string;
	case  9u: PUT_STRING_WIDTH(string, "09", 2); return string;
	case  8u: PUT_STRING_WIDTH(string, "08", 2); return string;
	case  7u: PUT_STRING_WIDTH(string, "07", 2); return string;
	case  6u: PUT_STRING_WIDTH(string, "06", 2); return string;
	case  5u: PUT_STRING_WIDTH(string, "05", 2); return string;
	case  4u: PUT_STRING_WIDTH(string, "04", 2); return string;
	case  3u: PUT_STRING_WIDTH(string, "03", 2); return string;
	case  2u: PUT_STRING_WIDTH(string, "02", 2); return string;
	default:  PUT_STRING_WIDTH(string, "01", 2); return string;
	}
}

inline char *
put_day(char *restrict string,
	const unsigned int day)
{
	switch (day) {
	case 31u: PUT_STRING_WIDTH(string, "31", 2); return string;
	case 30u: PUT_STRING_WIDTH(string, "30", 2); return string;
	case 29u: PUT_STRING_WIDTH(string, "29", 2); return string;
	case 28u: PUT_STRING_WIDTH(string, "28", 2); return string;
	case 27u: PUT_STRING_WIDTH(string, "27", 2); return string;
	case 26u: PUT_STRING_WIDTH(string, "26", 2); return string;
	case 25u: PUT_STRING_WIDTH(string, "25", 2); return string;
	case 24u: PUT_STRING_WIDTH(string, "24", 2); return string;
	case 23u: PUT_STRING_WIDTH(string, "23", 2); return string;
	case 22u: PUT_STRING_WIDTH(string, "22", 2); return string;
	case 21u: PUT_STRING_WIDTH(string, "21", 2); return string;
	case 20u: PUT_STRING_WIDTH(string, "20", 2); return string;
	case 19u: PUT_STRING_WIDTH(string, "19", 2); return string;
	case 18u: PUT_STRING_WIDTH(string, "18", 2); return string;
	case 17u: PUT_STRING_WIDTH(string, "17", 2); return string;
	case 16u: PUT_STRING_WIDTH(string, "16", 2); return string;
	case 15u: PUT_STRING_WIDTH(string, "15", 2); return string;
	case 14u: PUT_STRING_WIDTH(string, "14", 2); return string;
	case 13u: PUT_STRING_WIDTH(string, "13", 2); return string;
	case 12u: PUT_STRING_WIDTH(string, "12", 2); return string;
	case 11u: PUT_STRING_WIDTH(string, "11", 2); return string;
	case 10u: PUT_STRING_WIDTH(string, "10", 2); return string;
	case  9u: PUT_STRING_WIDTH(string, "09", 2); return string;
	case  8u: PUT_STRING_WIDTH(string, "08", 2); return string;
	case  7u: PUT_STRING_WIDTH(string, "07", 2); return string;
	case  6u: PUT_STRING_WIDTH(string, "06", 2); return string;
	case  3u: PUT_STRING_WIDTH(string, "03", 2); return string;
	case  2u: PUT_STRING_WIDTH(string, "02", 2); return string;
	default:  PUT_STRING_WIDTH(string, "01", 2); return string;
	}
}

inline char *
put_time(char *restrict string,
	 const unsigned int hour,
	 const unsigned int minute,
	 const unsigned int second)
{
	/* "HH:" */
	switch (hour) {
	case 23u: PUT_STRING_WIDTH(string, "23:", 3); break;
	case 22u: PUT_STRING_WIDTH(string, "22:", 3); break;
	case 21u: PUT_STRING_WIDTH(string, "21:", 3); break;
	case 20u: PUT_STRING_WIDTH(string, "20:", 3); break;
	case 19u: PUT_STRING_WIDTH(string, "19:", 3); break;
	case 18u: PUT_STRING_WIDTH(string, "18:", 3); break;
	case 17u: PUT_STRING_WIDTH(string, "17:", 3); break;
	case 16u: PUT_STRING_WIDTH(string, "16:", 3); break;
	case 15u: PUT_STRING_WIDTH(string, "15:", 3); break;
	case 14u: PUT_STRING_WIDTH(string, "14:", 3); break;
	case 13u: PUT_STRING_WIDTH(string, "13:", 3); break;
	case 12u: PUT_STRING_WIDTH(string, "12:", 3); break;
	case 11u: PUT_STRING_WIDTH(string, "11:", 3); break;
	case 10u: PUT_STRING_WIDTH(string, "10:", 3); break;
	case  9u: PUT_STRING_WIDTH(string, "09:", 3); break;
	case  8u: PUT_STRING_WIDTH(string, "08:", 3); break;
	case  7u: PUT_STRING_WIDTH(string, "07:", 3); break;
	case  6u: PUT_STRING_WIDTH(string, "06:", 3); break;
	case  5u: PUT_STRING_WIDTH(string, "05:", 3); break;
	case  4u: PUT_STRING_WIDTH(string, "04:", 3); break;
	case  3u: PUT_STRING_WIDTH(string, "03:", 3); break;
	case  2u: PUT_STRING_WIDTH(string, "02:", 3); break;
	case  1u: PUT_STRING_WIDTH(string, "01:", 3); break;
	default:  PUT_STRING_WIDTH(string, "00:", 3);
	}

	/* "MM:" */
	switch (minute) {
	case 59u: PUT_STRING_WIDTH(string, "59:", 3); break;
	case 58u: PUT_STRING_WIDTH(string, "58:", 3); break;
	case 57u: PUT_STRING_WIDTH(string, "57:", 3); break;
	case 56u: PUT_STRING_WIDTH(string, "56:", 3); break;
	case 55u: PUT_STRING_WIDTH(string, "55:", 3); break;
	case 54u: PUT_STRING_WIDTH(string, "54:", 3); break;
	case 53u: PUT_STRING_WIDTH(string, "53:", 3); break;
	case 52u: PUT_STRING_WIDTH(string, "52:", 3); break;
	case 51u: PUT_STRING_WIDTH(string, "51:", 3); break;
	case 50u: PUT_STRING_WIDTH(string, "50:", 3); break;
	case 49u: PUT_STRING_WIDTH(string, "49:", 3); break;
	case 48u: PUT_STRING_WIDTH(string, "48:", 3); break;
	case 47u: PUT_STRING_WIDTH(string, "47:", 3); break;
	case 46u: PUT_STRING_WIDTH(string, "46:", 3); break;
	case 45u: PUT_STRING_WIDTH(string, "45:", 3); break;
	case 44u: PUT_STRING_WIDTH(string, "44:", 3); break;
	case 43u: PUT_STRING_WIDTH(string, "43:", 3); break;
	case 42u: PUT_STRING_WIDTH(string, "42:", 3); break;
	case 41u: PUT_STRING_WIDTH(string, "41:", 3); break;
	case 40u: PUT_STRING_WIDTH(string, "40:", 3); break;
	case 39u: PUT_STRING_WIDTH(string, "39:", 3); break;
	case 38u: PUT_STRING_WIDTH(string, "38:", 3); break;
	case 37u: PUT_STRING_WIDTH(string, "37:", 3); break;
	case 36u: PUT_STRING_WIDTH(string, "36:", 3); break;
	case 35u: PUT_STRING_WIDTH(string, "35:", 3); break;
	case 34u: PUT_STRING_WIDTH(string, "34:", 3); break;
	case 33u: PUT_STRING_WIDTH(string, "33:", 3); break;
	case 32u: PUT_STRING_WIDTH(string, "32:", 3); break;
	case 31u: PUT_STRING_WIDTH(string, "31:", 3); break;
	case 30u: PUT_STRING_WIDTH(string, "30:", 3); break;
	case 29u: PUT_STRING_WIDTH(string, "29:", 3); break;
	case 28u: PUT_STRING_WIDTH(string, "28:", 3); break;
	case 27u: PUT_STRING_WIDTH(string, "27:", 3); break;
	case 26u: PUT_STRING_WIDTH(string, "26:", 3); break;
	case 25u: PUT_STRING_WIDTH(string, "25:", 3); break;
	case 24u: PUT_STRING_WIDTH(string, "24:", 3); break;
	case 23u: PUT_STRING_WIDTH(string, "23:", 3); break;
	case 22u: PUT_STRING_WIDTH(string, "22:", 3); break;
	case 21u: PUT_STRING_WIDTH(string, "21:", 3); break;
	case 20u: PUT_STRING_WIDTH(string, "20:", 3); break;
	case 19u: PUT_STRING_WIDTH(string, "19:", 3); break;
	case 18u: PUT_STRING_WIDTH(string, "18:", 3); break;
	case 17u: PUT_STRING_WIDTH(string, "17:", 3); break;
	case 16u: PUT_STRING_WIDTH(string, "16:", 3); break;
	case 15u: PUT_STRING_WIDTH(string, "15:", 3); break;
	case 14u: PUT_STRING_WIDTH(string, "14:", 3); break;
	case 13u: PUT_STRING_WIDTH(string, "13:", 3); break;
	case 12u: PUT_STRING_WIDTH(string, "12:", 3); break;
	case 11u: PUT_STRING_WIDTH(string, "11:", 3); break;
	case 10u: PUT_STRING_WIDTH(string, "10:", 3); break;
	case  9u: PUT_STRING_WIDTH(string, "09:", 3); break;
	case  8u: PUT_STRING_WIDTH(string, "08:", 3); break;
	case  7u: PUT_STRING_WIDTH(string, "07:", 3); break;
	case  6u: PUT_STRING_WIDTH(string, "06:", 3); break;
	case  5u: PUT_STRING_WIDTH(string, "05:", 3); break;
	case  4u: PUT_STRING_WIDTH(string, "04:", 3); break;
	case  3u: PUT_STRING_WIDTH(string, "03:", 3); break;
	case  2u: PUT_STRING_WIDTH(string, "02:", 3); break;
	case  1u: PUT_STRING_WIDTH(string, "01:", 3); break;
	default:  PUT_STRING_WIDTH(string, "00:", 3);
	}

	/* "SS" */
	switch (second) {
	case 59u: PUT_STRING_WIDTH(string, "59", 2); return string;
	case 58u: PUT_STRING_WIDTH(string, "58", 2); return string;
	case 57u: PUT_STRING_WIDTH(string, "57", 2); return string;
	case 56u: PUT_STRING_WIDTH(string, "56", 2); return string;
	case 55u: PUT_STRING_WIDTH(string, "55", 2); return string;
	case 54u: PUT_STRING_WIDTH(string, "54", 2); return string;
	case 53u: PUT_STRING_WIDTH(string, "53", 2); return string;
	case 52u: PUT_STRING_WIDTH(string, "52", 2); return string;
	case 51u: PUT_STRING_WIDTH(string, "51", 2); return string;
	case 50u: PUT_STRING_WIDTH(string, "50", 2); return string;
	case 49u: PUT_STRING_WIDTH(string, "49", 2); return string;
	case 48u: PUT_STRING_WIDTH(string, "48", 2); return string;
	case 47u: PUT_STRING_WIDTH(string, "47", 2); return string;
	case 46u: PUT_STRING_WIDTH(string, "46", 2); return string;
	case 45u: PUT_STRING_WIDTH(string, "45", 2); return string;
	case 44u: PUT_STRING_WIDTH(string, "44", 2); return string;
	case 43u: PUT_STRING_WIDTH(string, "43", 2); return string;
	case 42u: PUT_STRING_WIDTH(string, "42", 2); return string;
	case 41u: PUT_STRING_WIDTH(string, "41", 2); return string;
	case 40u: PUT_STRING_WIDTH(string, "40", 2); return string;
	case 39u: PUT_STRING_WIDTH(string, "39", 2); return string;
	case 38u: PUT_STRING_WIDTH(string, "38", 2); return string;
	case 37u: PUT_STRING_WIDTH(string, "37", 2); return string;
	case 36u: PUT_STRING_WIDTH(string, "36", 2); return string;
	case 35u: PUT_STRING_WIDTH(string, "35", 2); return string;
	case 34u: PUT_STRING_WIDTH(string, "34", 2); return string;
	case 33u: PUT_STRING_WIDTH(string, "33", 2); return string;
	case 32u: PUT_STRING_WIDTH(string, "32", 2); return string;
	case 31u: PUT_STRING_WIDTH(string, "31", 2); return string;
	case 30u: PUT_STRING_WIDTH(string, "30", 2); return string;
	case 29u: PUT_STRING_WIDTH(string, "29", 2); return string;
	case 28u: PUT_STRING_WIDTH(string, "28", 2); return string;
	case 27u: PUT_STRING_WIDTH(string, "27", 2); return string;
	case 26u: PUT_STRING_WIDTH(string, "26", 2); return string;
	case 25u: PUT_STRING_WIDTH(string, "25", 2); return string;
	case 24u: PUT_STRING_WIDTH(string, "24", 2); return string;
	case 23u: PUT_STRING_WIDTH(string, "23", 2); return string;
	case 22u: PUT_STRING_WIDTH(string, "22", 2); return string;
	case 21u: PUT_STRING_WIDTH(string, "21", 2); return string;
	case 20u: PUT_STRING_WIDTH(string, "20", 2); return string;
	case 19u: PUT_STRING_WIDTH(string, "19", 2); return string;
	case 18u: PUT_STRING_WIDTH(string, "18", 2); return string;
	case 17u: PUT_STRING_WIDTH(string, "17", 2); return string;
	case 16u: PUT_STRING_WIDTH(string, "16", 2); return string;
	case 15u: PUT_STRING_WIDTH(string, "15", 2); return string;
	case 14u: PUT_STRING_WIDTH(string, "14", 2); return string;
	case 13u: PUT_STRING_WIDTH(string, "13", 2); return string;
	case 12u: PUT_STRING_WIDTH(string, "12", 2); return string;
	case 11u: PUT_STRING_WIDTH(string, "11", 2); return string;
	case 10u: PUT_STRING_WIDTH(string, "10", 2); return string;
	case  9u: PUT_STRING_WIDTH(string, "09", 2); return string;
	case  8u: PUT_STRING_WIDTH(string, "08", 2); return string;
	case  7u: PUT_STRING_WIDTH(string, "07", 2); return string;
	case  6u: PUT_STRING_WIDTH(string, "06", 2); return string;
	case  5u: PUT_STRING_WIDTH(string, "05", 2); return string;
	case  4u: PUT_STRING_WIDTH(string, "04", 2); return string;
	case  3u: PUT_STRING_WIDTH(string, "03", 2); return string;
	case  2u: PUT_STRING_WIDTH(string, "02", 2); return string;
	case  1u: PUT_STRING_WIDTH(string, "01", 2); return string;
	default:  PUT_STRING_WIDTH(string, "00", 2); return string;
	}
}

inline void
timestamp_string_init(char *restrict string,
		      const struct Timestamp *const restrict timestamp)
{
	string = put_year(string,
			  timestamp->year);

	*string = '-';
	++string;

	string = put_month(string,
			   timestamp->month);

	*string = '-';
	++string;

	/* "DD " */
	string = put_day(string,
			 timestamp->day);

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

inline const char *
month_string(const unsigned int month)
{
	return month_map[month];
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

	string = put_day(string,
			 timestamp->day);

	*string = ' ';
	++string;

	PUT_STRING_WIDTH(string,
			 month_string(timestamp->month),
			 3);

	*string = ' ';
	++string;

	string = put_year(string,
			  timestamp->year);

	*string = ' ';
	++string;

	string = put_time(string,
			  timestamp->hour,
			  timestamp->minute,
			  timestamp->second);

	SET_STRING_WIDTH(string,
			 " GMT",
			 4);

	return string + 4;
}

#endif /* ifndef NETWORK_TIME_TIME_STRING_H_ */
