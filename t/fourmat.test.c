#include <test.h>

#include "fourmat.h"

SUITE_FILE

static char buf[10];

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
