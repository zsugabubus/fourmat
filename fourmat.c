#include <langinfo.h>

#include "fourmat.h"

#define array_len(a) (sizeof a  / sizeof *a)

static void
dec_sep(char *c)
{
	char const *const item = nl_langinfo(RADIXCHAR);
	*c = '\0' == item[1] ? item[0] : '.';
}

int
fmt_time(char *str, uint32_t time)
{
	static char const
	TIME_UNITS[] = {
		's',
		'm',
		'h',
		'd',
		'w',
		'y'
	};
	static uint32_t const
	TIME_BASE[] = {
		1,
		60,
		60 * 60,
		60 * 60 * 24,
		60 * 60 * 24 * 7,
		60 * 60 * 24 * 7 * 52,
		UINT32_MAX
	};

	if (time >= 100 * TIME_BASE[array_len(TIME_BASE) - 2]) {
		str[0] = 'n';
		str[1] = 'e';
		str[2] = 'v';
		str[3] = 'e';
		str[4] = 'r';
	} else {
		uint8_t i, j;

		for (i = 0; (TIME_BASE + 1)[i] < time; ++i)
			;

		/* t1 UNIT[i] [ t2 UNIT[j] ] */
		uint8_t t1, t2;

		t1 = time / TIME_BASE[i];
		time %= TIME_BASE[i];

		for (j = i; 0 < j && 0 == (t2 = time / TIME_BASE[--j]);)
			time %= TIME_BASE[j];

		if ((t1 < 10 || t2 < 10) && 0 < t2) {
			str[3] = '0' + (t2 % 10);
			str[4] = TIME_UNITS[j];

			if (t2 < 10) {
				str[0] = t1 >= 10 ? '0' + (t1 / 10) : ' ';
				str -= 2;
			} else {
				str[0] = ' ';
				str[2] = '0' + (t2 / 10);
				str -= 3;
			}
		} else {
			str[0] = ' ';
			str[1] = ' ';
			str[2] = t1 >= 10 ? '0' + (t1 / 10) : ' ';
		}
		str[3] = '0' + (t1 % 10);
		str[4] = TIME_UNITS[i];
	}
	return 5;
}

static int
fmt_decimal(char *str, uint64_t n, char const *UNITS, uint64_t const *BASE)
{
	int i = 0;

	while (n >= 1000 * BASE[i++])
		;

	n *= BASE[2];
	n /= (BASE - 1)[i];

	if (' ' == (UNITS - 1)[i])
		*str++ = ' ';

	if (n >= 10 * BASE[2] || 1 == (BASE - 1)[i]) {
		n /= BASE[2];
		str[0] = n >= 100 ? '0' + (n / 100)      : ' ';
		str[1] = n >= 10  ? '0' + (n % 100 / 10) : ' ';
		str[2] = '0' + (n % 10);
	} else {
		n *= 10;
		n /= BASE[2];
		str[0] = '0' + (n / 10);
		dec_sep(&str[1]);
		str[2] = '0' + (n % 10);
	}

	if (' ' != (UNITS - 1)[i])
		str[3] = (UNITS - 1)[i];

	return 4;
}

int
fmt_speed(char *str, uint64_t num)
{
	static char const
	BYTES_METRIC_UNITS[] = { ' ', 'B', 'k', 'M', 'G', 'T', 'Y' };
	static uint64_t const
	BYTES_METRIC_BASE[] = { 1, 1e0, 1e3, 1e6, 1e9, 1e12, 1e15, 1e18 };

	return fmt_decimal(str, num, BYTES_METRIC_UNITS, BYTES_METRIC_BASE);
}

int
fmt_space(char *str, uint64_t num)
{
#define E(n) (UINT64_C(1) << (10U * n))
	static char const
	BYTES_IEC_UNITS[] = { ' ', 'B', 'K', 'M', 'G', 'T', 'Y' };
	static uint64_t const
	BYTES_IEC_BASE[] = { 1, E(0), E(1), E(2), E(3), E(4), E(5), E(6) };
#undef E

	return fmt_decimal(str, num, BYTES_IEC_UNITS, BYTES_IEC_BASE);
}

int
fmt_number(char *str, uint64_t num)
{
	static char *const
	NUMBER_UNITS = "kMGT";

	if (num < 10000) {
		str[0] = num >= 1000 ? '0' + (num / 1000) : ' ';
	} else {
		char *unit = NUMBER_UNITS;

		while ((num /= 1000) >= 1000)
			++unit;

		str[3] = *unit;
		--str;
	}

	str[1] = num >= 100 ? '0' + (num % 1000 / 100) : ' ';
	str[2] = num >= 10  ? '0' + (num % 100  / 10)  : ' ';
	str[3] = '0' + (num % 10);

	return 4;
}

int
fmt_percent(char *str, uint64_t num, uint64_t total)
{
#define PERCENT * 100
	uint32_t p = total > 0 ? num * (100 PERCENT) / total : 0;
	if (p < 10 PERCENT) {
		/* 9.99% */
		str[0] = '0' + (p / 100);
		dec_sep(&str[1]);
		str[2] = '0' + (p % 100 / 10);
		str[3] = '0' + (p % 10);
		str[4] = '%';
	} else if (p < 100 PERCENT) {
		/* 99.9% */
		p /= 10;
		str[0] = '0' + (p / 100);
		str[1] = '0' + (p % 100 / 10);
		dec_sep(&str[2]);
		str[3] = '0' + (p % 10);
		str[4] = '%';
	} else if (p < 10000 PERCENT) {
		/* 999% */
		p /= 100;
		str[0] = p >= 1000 ? '0' + (p / 1000) : ' ';
		str[1] = '0' + (p % 1000 / 100);
		str[2] = '0' + (p % 100 / 10);
		str[3] = '0' + (p % 10);
		str[4] = '%';
	} else {
		/* x   9 */
		p /= 100 PERCENT;
		str[0] = 'x';
		return 1 + fmt_number(str + 1, p);
	}
#undef PERCENT
	return 5;
}
