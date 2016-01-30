#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "engfmt.h"

/* Copy b into buffer a of length len.  A will always end up NUL terminated. */

static char *zlcpy(char *a, ptrdiff_t len, const char *b)
{
	char *org = a;
	assert(len != 0);
	--len;
	while (len && *b) {
		*a++ = *b++;
		--len;
	}
	*a = 0;
	return org;
}

/* Insert commas (or any separator) into a floating point ascii number */

char *insert_commas(char *dst, const char *src, char sep)
{
	char *org = dst;
	int leading; /* Number of leading digits */
	int trailing;
	int n;
	const char *s;
	/* First pass: calculate number of leading and trailing digits */
	s = src;
	leading = 0;
	trailing = 0;
	if (*s == '-') ++s;
	else if (*s == '+') ++s;
	while (*s >= '0' && *s <= '9') {
		++s;
		++leading;
	}
	if (*s == '.') {
		++s;
		while (*s >= '0' && *s <= '9') {
			++s;
			++trailing;
		}
	}
	/* Second pass: copy while inserting */
	s = src;
	n = 0;
	if (*s == '-') {
		*dst++ = *s++;
	} else if (*s == '+') {
		*dst++ = *s++;
	}
	while (*s >= '0' && *s <= '9') {
		*dst++ = *s++;
		--leading;
		if (leading != 0 && (leading % 3 == 0))
			*dst++ = sep;
	}
	if (*s == '.') {
		*dst++ = *s++;
		while (*s >= '0' && *s <= '9') {
			*dst++ = *s++;
			++n;
			if (n != trailing && (n % 3 == 0))
				*dst++ = sep;
		}
	}
	while (*s) {
		*dst++ = *s++;
	}
	*dst = 0;
	return org;
}

/* Convert floating point ASCII number in s
 * into engineering format- so that the exponent
 * is a multiple of 3 */

char *engfmt(char *d, size_t d_len, const char *s)
{
	const char *s_org = s;
	char *d_org = d;
	char a[128];
	int a_len;
	int sign;
	int dp;
	int exp;
	int exp_sign;
	int x;
	int flg = 0;

	--d_len; /* For terminator */

	/* Get sign of number */
	if (*s == '-') {
		sign = 1;
		++s;
	} else if (*s == '+') {
		sign = 0;
		++s;
	} else {
		sign = 0;
	}

	/* Read digits before decimal point */
	/* Skip leading zeros */
	while (*s == '0') {
		flg = 1;
		++s;
	}
	a_len = 0;
	while (*s >= '0' && *s <= '9') {
		a[a_len++] = *s++;
		flg = 1;
	}
	/* Decimal point? */
	dp = 0;
	if (*s == '.') {
		flg = 1;
		++s;
		/* If we don't have any digits yet, trim leading zeros */
		if (!a_len) {
			while (*s == '0') {
				++s;
				++dp;
			}
		}
		if (*s >= '0' && *s <= '9') {
			while (*s >= '0' && *s <= '9') {
				a[a_len++] = *s++;
				++dp;
			}
		} else {
			/* No non-zero digits at all? */
			dp  = 0;
		}
	}
	/* Trim trailing zeros */
	while (a_len && a[a_len - 1] == '0') {
		--a_len;
		--dp;
	}

	/* Exponent? */
	if (*s == 'e' || *s == 'E') {
		++s;
		if (*s == '-') {
			++s;
			exp_sign = 1;
		} else if (*s == '+') {
			++s;
			exp_sign = 0;
		} else {
			exp_sign = 0;
		}
		exp = 0;
		while  (*s >= '0' && *s <= '9') {
			exp = exp * 10 + *s++ - '0';
		}
	} else {
		exp = 0;
		exp_sign = 0;
	}

	/* s should be at end of number now */
	if (!flg) { /* No digits found? */
		zlcpy(d, d_len, s_org);
		return d;
	}

	/* Sign of exponent */
	if (exp_sign)
		exp = -exp;
	/* Account of position of decimal point in exponent */
	exp -= dp;

	a[a_len] = 0;

	/* For engineering format, make expoenent a multiple of 3 such that
	   we have 1 - 3 leading digits */

	/* Don't assume modulus of negative number works consistently */
	if (exp < 0) {
		switch((-exp) % 3) {
			case 0: x = 0; break;
			case 1: x = 2; break;
			case 2: x = 1; break;
		}
	} else {
		x = (exp % 3);
	}

	/* Make exponent a multiple of 3 */
	exp -= x;

	/* Add zeros to mantissa to account for this */
	while (x--) {
		a[a_len++] = '0';
	}

	/* If number has no digits, add one now */
	if (!a_len)
		a[a_len++] = '0';

	/* Position decimal point near the left */
	dp = (a_len - 1) / 3;
	dp *= 3;

	/* Adjust exponent for this */
	exp += dp;

	/* Now print */
	if (sign && d_len) {
		*d++ = '-';
		--d_len;
	}

	/* Digits to left of decimal point */
	for (x = 0; x != a_len - dp; ++x) {
		if (d_len) {
			*d++ = a[x];
			--d_len;
		}
	}

	/* Any more digits? */
	if (dp) {
		if (d_len) {
			*d++ = '.';
			--d_len;
		}
		for (x = a_len - dp; x != a_len; ++x) {
			if (d_len) {
				*d++ = a[x];
				--d_len;
			}
		}
		/* Trim trailing zeros */
		while (d != d_org && d[-1] == '0') {
			--d;
			++d_len;
		}
	}

	/* Exponent? */
	if (exp) {
		snprintf(d, d_len + 1, "e%d", exp);
	} else {
		*d = 0;
	}

	return d_org;
}
