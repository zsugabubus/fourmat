#include <test.h>

#include "fourmat.h"

SUITE_FILE

static char buf[10];

TEST(fmt_time)
{
#define A(expect, seconds) do { \
	buf[fmt_time(buf, seconds)] = '\0'; \
	assert_equal(&buf, expect); \
} while (0)

#define SEC  * 1
#define MIN  * 60
#define HOUR * 60 MIN
#define DAY  * 24 HOUR
#define WEEK * 7 DAY
#define YEAR * 52 WEEK

	A("   0s", 0 SEC);
	A("   1s", 1 SEC);
	A("   9s", 9 SEC);
	A(" 1m1s", 1 MIN + 1 SEC);
	A("1m30s", 1 MIN + 30 SEC);
	A("10m1s", 10 MIN + 1 SEC);
	A("  10m", 10 MIN + 10 SEC);
	A("  60m", 60 MIN);
	A(" 1h1s", 1 HOUR + 1 SEC);
	A(" 1h1m", 1 HOUR + 1 MIN);
	A(" 1h1m", 1 HOUR + 1 MIN + 1 SEC);
	A("1h30m", 1 HOUR + 30 MIN);
	A("1h10m", 1 HOUR + 10 MIN);
	A("3y14w", 3 YEAR + 14 WEEK);
	A("21y2s", 21 YEAR + 2 SEC);
	A("never", -1);

#undef SEC
#undef MIN
#undef HOUR
#undef DAY
#undef WEEK
#undef YEAR

#undef A
}

TEST(fmt_speed)
{
#define A(expect, number) do { \
	buf[fmt_speed(buf, number)] = '\0'; \
	assert_equal(&buf, expect); \
} while (0)

	A("   0", 0);
	A("   1", 1);
	A("   9", 9);
	A("  99", 99);
	A(" 999", 999);
	A("1.0k", 1000);
	A("1.0k", 1023);
	A("1.0k", 1024);
	A("1.0k", 1099);
	A("1.1k", 1100);
	A("1.9k", 1999);
	A("1.0M", 1000 * 1000);
	A("1.0G", 1000 * 1000 * 1000);

#undef A
}

TEST(fmt_space)
{
#define A(expect, number) do { \
	buf[fmt_space(buf, number)] = '\0'; \
	assert_equal(&buf, expect); \
} while (0)

	A("   0", 0);
	A("   1", 1);
	A("   9", 9);
	A("  99", 99);
	A(" 999", 999);
	A("0.9K", 1000);
	A("0.9K", 1023);
	A("1.0K", 1024);
	A("1.5K", 1024 * 3 / 2);
	A("9.0K", 9 * 1024);
	A("9.9K", 9 * 1024 + 1023);
	A("0.9M", 1024 * 1023);
	A("0.9M", 1024 * 1024 - 1);
	A("1.0M", 1024 * 1024);
	A("1.0G", 1024 * 1024 * 1024);
	A("1.0T", UINT64_C(1024) * 1024 * 1024 * 1024);

#undef A
}

TEST(fmt_number)
{
#define A(expect, number) do { \
	buf[fmt_number(buf, number)] = '\0'; \
	assert_equal(&buf, expect); \
} while (0)

	A("   0", 0);
	A("   1", 1);
	A("   9", 9);
	A("  99", 99);
	A(" 999", 999);
	A("1000", 1000);
	A("1023", 1023);
	A("1024", 1024);
	A("9999", 9999);
	A(" 10k", 10 * 1000);
	A("999k", 1000 * 1000 - 1);
	A("  1M", 1000 * 1000);
	A("  1G", 1000 * 1000 * 1000);
	A("  1T", UINT64_C(1000) * 1000 * 1000 * 1000);

#undef A
}


TEST(fmt_percent)
{
#define A(expect, number, denominator) do { \
	buf[fmt_percent(buf, number, denominator)] = '\0'; \
	assert_equal(&buf, expect); \
} while (0)

	A("0.00%", 0, 0);
	A("0.00%", 1, 0);
	A("0.00%", 1, 1000000);
	A(" 100%", 1, 1);
	A("1.00%", 1, 100);
	A("3.14%", 314, 100 * 100);
	A("31.4%", 314,  10 * 100);
	A(" 314%", 314,   1 * 100);
	A(" 999%", 999, 100);
	A("9999%", 9999, 100);
	A("x9999", 9999, 1);
	A("x 10k", 10000, 1);
	A("x100k", 100000, 1);

#undef A
}
