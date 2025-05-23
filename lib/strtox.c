// SPDX-License-Identifier: GPL-2.0-only

#include <common.h>
#include <linux/ctype.h>

#include "kstrtox.h"

/**
 * simple_strtoull - convert a string to an unsigned long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtoull instead.
 */
unsigned long long simple_strtoull(const char *cp, char **endp,
				   unsigned int base)
{
	unsigned long long result;
	unsigned int rv;

	cp = _parse_integer_fixup_radix(cp, &base);
	rv = _parse_integer(cp, base, &result);
	/* FIXME */
	cp += (rv & ~KSTRTOX_OVERFLOW);

	if (endp)
		*endp = (char *)cp;

	return result;
}
EXPORT_SYMBOL(simple_strtoull);

/**
 * simple_strtoul - convert a string to an unsigned long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtoul instead.
 */
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base)
{
	return simple_strtoull(cp, endp, base);
}
EXPORT_SYMBOL(simple_strtoul);

/**
 * simple_strtol - convert a string to a signed long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtol instead.
 */
long simple_strtol(const char *cp, char **endp, unsigned int base)
{
	if (*cp == '-')
		return -simple_strtoul(cp + 1, endp, base);

	return simple_strtoul(cp, endp, base);
}
EXPORT_SYMBOL(simple_strtol);

/**
 * simple_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtoll instead.
 */
long long simple_strtoll(const char *cp, char **endp, unsigned int base)
{
	if (*cp == '-')
		return -simple_strtoull(cp + 1, endp, base);

	return simple_strtoull(cp, endp, base);
}
EXPORT_SYMBOL(simple_strtoll);

static unsigned int simple_strtouint(const char *cp, char **endp, unsigned int base)
{
	unsigned int ret;

	ret = simple_strtoull(cp, endp, base);
	if (ret != (unsigned int)ret) {
		if (endp)
			*endp = (char *)cp;
		return  0;
	}

	return ret;
}

s64 simple_strtofract(const char *cp, char **endp, u32 division)
{
	char *end = (char *)cp;
	s64 integer, fract, scale, result = 0;
	int fract_len = 0, sign = 1;

	switch (*cp) {
	case '-':
		sign = -1;
		fallthrough;
	case '+':
		cp++;
		break;
	}

	if (!isdigit(*cp))
		goto out;

	integer = simple_strtouint(cp, &end, 10);
	if (end == cp)
		goto out;

	if (*end == '.' || *end == ',') {
		cp = end + 1;
		fract = simple_strtouint(cp, &end, 10);
		fract_len = end - cp;
	}

	result = integer * division;

	scale = 1;
	for (int i = 0; i < fract_len; i++)
		scale *= 10;
	if (fract_len > 0)
		result += div64_u64(fract * division, scale);

out:
	if (endp)
		*endp = end;

	return sign * result;
}
EXPORT_SYMBOL(simple_strtofract);
