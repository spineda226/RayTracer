/**
 * MortonCode.h
 * 
 * Modified from Brent Williams
 */

#ifndef _MORTON_H_
#define _MORTON_H_

#include <stdint.h>
#include <limits.h>
#include <iostream>
#include <math.h>

uint32_t mortonCode(unsigned int x, unsigned int y, unsigned int z, unsigned int level);
void mortonCodeToXYZ(uint32_t mortonCode, unsigned int *x, unsigned int *y, unsigned int *z, unsigned int level);
void printBinary(uint32_t value);
void printBinary(uint32_t value, unsigned int level);

#endif