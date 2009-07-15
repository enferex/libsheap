/******************************************************************************
 * main.c 
 *
 * libsheap - Simple heap monitoring library.
 *            (Sheap Helps Examine Allocated Pointers)
 *
 * Copyright (C) 2009 Matt Davis (enferex) of 757Labs (www.757labs.com)
 *
 * main.c is part of libsheap.
 * libsheap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsheap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libsheap.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/


/* Special thanks to Google and stackoverflow.com where I learned about wrapping
 * routines in the GNU/Linux system.  Much of the wrapping code here was
 * paraphrased from Checker's post over at stackoverflow.com
 *
 * Thanks Checkers!
 * <http://stackoverflow.com/questions/262439/create-a-wrapper-function-for-malloc-and-free-in-c>
 */


#define _GNU_SOURCE
#include <features.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* Needs _GNU_SOURCE for RTLD_NEXT */
#include <dlfcn.h>


/* Cycle counter */
#ifdef ENABLE_CYCLE_COUNTER
#define GET_CYCLES(_cycles) \
{                                                     \
    uint32_t low, high;                               \
    asm volatile ("rdtsc\n" : "=d"(high), "=a"(low)); \
    _cycles = high;                                   \
    _cycles = (_cycles << 32) | low;                  \
}

#else
#define GET_CYCLES(_x) /* Do nothing */
#endif


/* Capture allocations */
void *malloc(size_t size)
{
    size_t actual;
    void *addr;
    uint64_t st, en;
    void *(*real_malloc)(size_t);

    st = en = 0;
    real_malloc = dlsym(RTLD_NEXT, "malloc");

    /* Call the underlying malloc routine for memory */
    GET_CYCLES(st);
    addr = real_malloc(size);
    GET_CYCLES(en);

    /* Actual amount of bytes allocated for this chunk for alignment */
    actual = *(size_t *)(addr - (sizeof(size_t)));

    /* Should negate the first 3-loworder bits which glibc malloc uses for
     * flags.
     */
    actual = (actual & ~0x7);

    printf("[%lu] %p allocated %u of %u bytes requested for %llu cycles\n",
            time(NULL), addr, 
            (unsigned)actual, (unsigned)size,
            (unsigned long long)en - st);

    fflush(stdout);

    return addr;
}


/* Capture free */
void free(void *addr)
{
    void (*real_free)(void *);

    real_free = dlsym(RTLD_NEXT, "free");

    printf("[%lu] %p free\n", time(NULL), addr);
    fflush(stdout);

    /* Call the underlying free implementation */
    real_free(addr);
}
