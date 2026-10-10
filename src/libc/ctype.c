/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <ctype.h>

int
isprint(
	int ch
	)
{
	return ((ch >= ' ') && (ch <= '~'));
}

int
isgraph(
	int ch
	)
{
	return ((ch > ' ') && (ch <= '~'));
}

int
isspace(
	int ch
	)
{
	return ((ch == ' ') || ((ch >= '\t') && (ch <= '\r')));
}

int
isblank(
	int ch
	)
{
	return ((ch == ' ') || (ch == '\t'));
}

int
isalpha(
	int ch
	)
{
	return (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')));
}

int
isdigit(
	int ch
	)
{
	return ((ch >= '0') && (ch <= '9'));
}

int
tolower(
	int ch
	)
{
	if ((ch >= 'A') && (ch <= 'Z')) {
		ch += 'a' - 'A';
	}

	return (ch);
}

int
toupper(
	int ch
	)
{
	if ((ch >= 'a') && (ch <= 'z')) {
		ch += 'A' - 'a';
	}

	return (ch);
}

int
islower(
	int ch
	)
{
	return ((ch >= 'a') && (ch <= 'z'));
}

int
isupper(
	int ch
	)
{
	return ((ch >= 'A') && (ch <= 'Z'));
}

int
isalnum(
	int ch
	)
{
	return (isalpha(ch) || isdigit(ch));
}

int
iscntrl(
	int ch
	)
{
	return (((ch >= '\0') && (ch <= '\x1F')) || (ch == '\x7F'));
}

int
ispunct(
	int ch
	)
{
	return (isprint(ch) && !isalnum(ch) && !isspace(ch));
}

int
isxdigit(
	int ch
	)
{
	return (isdigit(ch) ||
		((ch >= 'A') && (ch <= 'F')) ||
		((ch >= 'a') && (ch <= 'f')));
}
