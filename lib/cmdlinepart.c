/*
 * command line partition parsing code
 *
 * Copyright (c) 2015 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <common.h>
#include <driver.h>
#include <fs.h>
#include <linux/err.h>
#include <cmdlinepart.h>

#define SIZE_REMAINING ((loff_t)-1)

int cmdlinepart_do_parse_one(const char *devname, const char *partstr,
				 const char **endp, loff_t *offset,
				 loff_t devsize, loff_t *retsize,
				 unsigned int partition_flags)
{
	loff_t size;
	char *end;
	char buf[PATH_MAX] = {};
	unsigned long flags = DEVFS_PARTITION_FOR_FIXUP;
	size_t partname_len = 0;
	struct cdev *cdev;

	memset(buf, 0, PATH_MAX);

	if (*partstr == '-') {
		size = SIZE_REMAINING;
		end = (char *)partstr + 1;
	} else {
		size = strtoull_suffix(partstr, &end, 0);
	}

	if (*end == '@')
		*offset = strtoull_suffix(end+1, &end, 0);

	if (size == SIZE_REMAINING)
		size = devsize - *offset;

	partstr = end;

	if (*partstr == '(') {
		partstr++;
		end = strchr((char *) partstr, ')');
		if (!end) {
			printf("could not find matching ')'\n");
			return -EINVAL;
		}

		if ((partition_flags & CMDLINEPART_ADD_DEVNAME) &&
				strncmp(devname, partstr, strlen(devname)))
			sprintf(buf, "%s.", devname);
		partname_len = end - partstr;
		memcpy(buf + strlen(buf), partstr, partname_len);

		end++;
	}

	if (size + *offset > devsize) {
		printf("%s: partition end is beyond device\n", buf);
		return -EINVAL;
	}

	partstr = end;

	if (*partstr == 'r' && *(partstr + 1) == 'o') {
		flags |= DEVFS_PARTITION_READONLY;
		end = (char *)(partstr + 2);
	}

	if (partition_flags & CMDLINEPART_FORCE)
		flags |= DEVFS_PARTITION_CAN_OVERLAP;

	if (endp)
		*endp = end;

	*retsize = size;

	if (!partname_len) {
		printf("partition name can't be empty\n");
		return -EINVAL;
	}

	cdev = devfs_add_partition(devname, *offset, size, flags, buf);
	if (IS_ERR(cdev)) {
		printf("cannot create %s: %pe\n", buf, cdev);
		return PTR_ERR(cdev);
	}

	return 0;
}

int cmdlinepart_do_parse(const char *devname, const char *parts, loff_t devsize,
		unsigned partition_flags)
{
	loff_t offset = 0;
	int ret;

	if (!parts || *parts == '\0')
		return 0;

	while (1) {
		loff_t size = 0;

		ret = cmdlinepart_do_parse_one(devname, parts, &parts, &offset,
				devsize, &size, partition_flags);
		if (ret)
			return ret;

		offset += size;
		if (!*parts)
			break;

		if (*parts != ',') {
			printf("parse error\n");
			return -EINVAL;
		}
		parts++;
	}

	return 0;
}
