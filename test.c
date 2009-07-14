/******************************************************************************
 * test.c 
 *
 * libsheap - Simple heap monitoring library.
 *            (Sheap Helps Examine Allocated Pointers)
 *
 * Copyright (C) 2009 Matt Davis (enferex) of 757Labs (www.757labs.com)
 *
 * test.c is part of libsheap.
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


#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int   i;
    char *c;

    printf("Testing...\n");

    for (i=0; i<10; ++i)
    {
        c = malloc(23);
        free(c);
    }

    return 0;
}
