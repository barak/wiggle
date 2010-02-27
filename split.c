/*
 * wiggle - apply rejected patches
 *
 * Copyright (C) 2003 Neil Brown <neilb@cse.unsw.edu.au>
 *
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    Author: Neil Brown
 *    Email: <neilb@cse.unsw.edu.au>
 *    Paper: Neil Brown
 *           School of Computer Science and Engineering
 *           The University of New South Wales
 *           Sydney, 2052
 *           Australia
 */

/*
 * split a stream into words or line
 * When splitting into words we can either be approximate or precise.
 *  Precise mode includes every char in a word.
 *  Approximate mode excluses white-space words and might unite some special chars
 *
 * In general, a word is one of:
 *    string of [A-Za-z0-9_]
 *    or string of [ \t]
 *    or single char.
 *
 * A line is any string that ends with \n
 * 
 * As a special case to allow proper aligning of multiple chunks
 * in a patch, a word starting \0 will include 5 chars and a newline
 *
 *
 * We make two passes through the stream.
 * Firstly we count the number of item so an array can be allocated,
 * then we store start and length of each item in the array
 *
 */

#include	"wiggle.h"
#include	<stdlib.h>
#include	<ctype.h>
#include	<malloc.h>
#define BITS_PER_LONG 32

#include "hash.h"

static int split_internal(char *start, char *end, int type, struct elmnt *list, int reverse)
{
	int cnt = 0;

	while (start < end) {
		char *cp = start;

		if (*cp == '\0' && cp+16 < end && cp[18] == '\n') {
			/* special word */
			cp += 19;
		} else switch(type) {
		case ByLine:
			while (cp < end && *cp != '\n')
				cp++;
			if (cp<end) cp++;
			break;
		case ByWord:
		case ApproxWord:
			if (isalnum(*cp) || *cp == '_') {
				do cp++;
				while (cp<end && (isalnum(*cp)  || *cp == '_'));
			} else if (*cp == ' ' || *cp == '\t') {
				do cp++;
				while (cp<end && (*cp == ' ' || *cp == '\t'));
			} else
				cp++;
			break;
		}
		if (type != ApproxWord || *start =='\0' ||
		    (isalnum(*start) || *start == '_')) {
			if (list) {
				if (reverse) list--;
				list->start = start;
				list->len = cp-start;
				if (*start)
					list->hash = hash_mem(start, list->len, BITS_PER_LONG);
				else
					list->hash = atoi(start+1);
				if (!reverse) list++;
			}
			cnt++;
		}
		start = cp;
	}
	return cnt;
}

struct file split_stream(struct stream s, int type, int reverse)
{
	int cnt;
	struct file f;

	char *c, *end;

	end = s.body+s.len;
	c = s.body;

	cnt = split_internal(c, end, type, NULL, reverse);
/*	fprintf(stderr, "cnt %d\n", cnt);*/
	f.list = malloc(cnt*sizeof(struct elmnt));

	f.elcnt = split_internal(c, end, type, f.list + reverse*cnt, reverse);
	return f;
}
