/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _CTYPE_H
#define _CTYPE_H

static inline int
isprint(int ch)
{
	return ((ch >= ' ') && (ch <= '~'));
}

static inline int
isgraph(int ch)
{
	return ((ch > ' ') && (ch <= '~'));
}

static inline int
isspace(int ch)
{
	return ((ch == ' ') || ((ch >= '\t') && (ch <= '\r')));
}

static inline int
isblank(int ch)
{
	return ((ch == ' ') || (ch == '\t'));
}

static inline int
isalpha(int ch)
{
	return (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')));
}

static inline int
isdigit(int ch)
{
	return ((ch >= '0') && (ch <= '9'));
}

static inline int
tolower(int ch)
{
	if ((ch >= 'A') && (ch <= 'Z')) {
		ch += 'a' - 'A';
	}

	return (ch);
}

static inline int
toupper(int ch)
{
	if ((ch >= 'a') && (ch <= 'z')) {
		ch += 'A' - 'a';
	}

	return (ch);
}

static inline int
islower(int ch)
{
	return ((ch >= 'a') && (ch <= 'z'));
}

static inline int
isupper(int ch)
{
	return ((ch >= 'A') && (ch <= 'Z'));
}

static inline int
isalnum(int ch)
{
	return (isalpha(ch) || isdigit(ch));
}

static inline int
iscntrl(int ch)
{
	return (((ch >= '\0') && (ch <= '\x1F')) || (ch == '\x7F'));
}

static inline int
ispunct(int ch)
{
	return (isprint(ch) && !isalnum(ch) && !isspace(ch));
}

static inline int
isxdigit(int ch)
{
	return (isdigit(ch) ||
		((ch >= 'A') && (ch <= 'F')) ||
		((ch >= 'a') && (ch <= 'f')));
}

#endif /* _CTYPE_H */
