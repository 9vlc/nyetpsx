/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _CTYPE_H
#define _CTYPE_H

int isprint(int ch);
int isgraph(int ch);
int isspace(int ch);
int isblank(int ch);
int isalpha(int ch);
int isdigit(int ch);
int tolower(int ch);
int toupper(int ch);
int islower(int ch);
int isupper(int ch);
int isalnum(int ch);
int iscntrl(int ch);
int ispunct(int ch);
int isxdigit(int ch);

#endif /* _CTYPE_H */
