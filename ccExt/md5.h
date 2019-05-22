/*
 * Copyright (c) 2004-2005 Sergey Lyubka <valenok@gmail.com>
 * All rights reserved
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Sergey Lyubka wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

#ifndef MD5_HEADER_INCLUDED
#define	MD5_HEADER_INCLUDED
//#include <stdint.h>

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MD5Context {
	uint32_t	buf[4];
	uint32_t	bits[2];
	unsigned char	in[64];
} MD5_CTX;

extern void MD5Init(MD5_CTX *ctx);
extern void MD5Update(MD5_CTX *ctx, unsigned char const *buf, unsigned len);
extern void MD5Final(unsigned char digest[16], MD5_CTX *ctx);

void md5_16h(const char *str, int length, unsigned char output[16]);
void md5_str(const char *str, int length, unsigned char output[32]);

#ifdef __cplusplus
}
#endif

#endif /*MD5_HEADER_INCLUDED */
