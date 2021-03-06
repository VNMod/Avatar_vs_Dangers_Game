/* Copyright (c) 2012, Texas Instruments Incorporated
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

*  Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

*  Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

*  Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include <ti/grlib/grlib.h>

static const unsigned char pixel_pacman4BPP_UNCOMP[] =
{
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x65, 0xf0, 0x00, 0x00, 0x53, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x65, 0xef, 0xef, 0xef, 0x0f, 0xef, 0x0f, 0xef, 0xe3, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x60, 0x00, 0xf0, 0xf4, 0x2c, 0xdc, 0xd2, 0x14, 0xf0, 0x00, 0xf3, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6f, 0x0f, 0xe4, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0x2f, 0x0f, 0xe3, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x6e, 0x00, 0x02, 0xdc, 0xc9, 0xdc, 0xd9, 0xdc, 0xd9, 0xdc, 0xc9, 0xdc, 0x2e, 0xf0, 0xf6, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0xef, 0x01, 0xcd, 0xdd, 0xdd, 0xdd, 0xcd, 0xdd, 0xcd, 0xdd, 0xcd, 0xdd, 0xdd, 0x2f, 0xef, 0x36, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x60, 0xfe, 0xc9, 0xdc, 0xd9, 0xdc, 0xd9, 0xdc, 0xd9, 0xdc, 0xd9, 0xdc, 0xd9, 0xdc, 0xc9, 0x50, 0x05, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0xef, 0xed, 0xcd, 0xdd, 0xcd, 0xdd, 0xcd, 0xdd, 0xcd, 0xcd, 0xcd, 0xdd, 0xcd, 0xdd, 0xcd, 0xc2, 0xef, 0x56, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x60, 0xf4, 0xd9, 0xd9, 0xc9, 0xd9, 0xc9, 0xd9, 0xc9, 0xd9, 0x91, 0x19, 0xc9, 0xd9, 0xc9, 0xc9, 0xd9, 0x30, 0x03, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0xef, 0x4d, 0x9d, 0xcd, 0x9d, 0xcd, 0x9d, 0xcd, 0x9d, 0xcf, 0xef, 0x0f, 0x9c, 0xcd, 0x9d, 0xcd, 0x9d, 0xc2, 0x0f, 0x36, 0x66, 0x66, 
0x66, 0x66, 0x6e, 0xfe, 0xc9, 0xd9, 0xc9, 0xc9, 0xc9, 0xd9, 0xc9, 0xc9, 0x40, 0xf0, 0x00, 0xf9, 0xc9, 0xd9, 0xc9, 0xd9, 0xc9, 0x30, 0x06, 0x66, 0x66, 
0x66, 0x66, 0x3f, 0xed, 0x9d, 0xcd, 0x9d, 0xcd, 0x9d, 0xcd, 0x9d, 0xcd, 0xef, 0x0f, 0xef, 0x0d, 0x9d, 0xcd, 0x9d, 0xcd, 0xcd, 0xcf, 0xe5, 0x66, 0x66, 
0x66, 0x66, 0xf0, 0x29, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0x00, 0x00, 0x00, 0x09, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0x00, 0x36, 0x66, 
0x66, 0x6e, 0x0e, 0x9c, 0x9d, 0x9c, 0x9d, 0x9c, 0x9d, 0x9c, 0x9d, 0x9c, 0x4f, 0xef, 0x0f, 0xec, 0x9d, 0x9c, 0x9d, 0x9c, 0x9c, 0xcd, 0x0f, 0x06, 0x66, 
0x66, 0x60, 0x09, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0x90, 0xf0, 0x00, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0x99, 0xf0, 0xf6, 0x66, 0x66, 
0x66, 0x5f, 0x4d, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9d, 0x9c, 0x9c, 0x9c, 0x14, 0x4d, 0x9c, 0x9d, 0x9c, 0x9d, 0x9d, 0x4f, 0x03, 0x66, 0x66, 0x66, 
0x66, 0x00, 0x29, 0x99, 0xc9, 0x99, 0x99, 0x99, 0xc9, 0x99, 0x99, 0x99, 0xc9, 0x99, 0x99, 0x99, 0xc9, 0x99, 0x99, 0x10, 0x00, 0x66, 0x66, 0x66, 0x66, 
0x66, 0xef, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0xcf, 0xef, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x65, 0xf4, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0xce, 0x0e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6f, 0x02, 0x99, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c, 0x91, 0xef, 0x46, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x60, 0x08, 0xa9, 0x98, 0xa9, 0x98, 0x99, 0x98, 0x99, 0x98, 0x99, 0x98, 0xa9, 0x98, 0xa9, 0x9a, 0xf0, 0x06, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6f, 0xe9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9c, 0xef, 0xe6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x60, 0xf9, 0x98, 0x99, 0x98, 0x99, 0x98, 0x99, 0x98, 0x98, 0x98, 0x99, 0x98, 0x9a, 0x4e, 0xf3, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6f, 0xe9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x1f, 0xe4, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x60, 0xe8, 0xa8, 0x98, 0xa8, 0xa8, 0xa8, 0x98, 0xa8, 0xa8, 0xa8, 0x98, 0xa8, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6f, 0xe9, 0x89, 0x89, 0x89, 0x89, 0x99, 0x89, 0x99, 0x89, 0x89, 0x89, 0x99, 0x2f, 0x05, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x60, 0xf8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xaa, 0x40, 0xf3, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6f, 0xea, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x82, 0xef, 0xe6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x60, 0x08, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa1, 0x00, 0x56, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6f, 0xe9, 0x89, 0x89, 0x89, 0x88, 0x89, 0x89, 0x89, 0x88, 0x89, 0x89, 0x89, 0x88, 0x89, 0x88, 0xa4, 0xef, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x6e, 0xf1, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0x20, 0xfe, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x63, 0x04, 0x88, 0x89, 0x88, 0x89, 0x88, 0x89, 0x88, 0x89, 0x88, 0x89, 0x88, 0x89, 0x88, 0x89, 0x88, 0x8a, 0x1f, 0xe3, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x00, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xaa, 0xf0, 0xf6, 0x66, 0x66, 0x66, 0x66, 
0x66, 0xef, 0x1a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x82, 0x0f, 0x46, 0x66, 0x66, 0x66, 
0x66, 0x30, 0xe8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa5, 0xf0, 0x36, 0x66, 0x66, 
0x66, 0x6f, 0xe1, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0xaf, 0xef, 0x36, 0x66, 
0x66, 0x63, 0x0e, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0x50, 0x06, 0x66, 
0x66, 0x66, 0xef, 0x48, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x88, 0x8a, 0x0f, 0x66, 0x66, 
0x66, 0x66, 0x60, 0xf1, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa0, 0x05, 0x66, 0x66, 
0x66, 0x66, 0x65, 0x0f, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x4f, 0xe6, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x00, 0xea, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa4, 0x00, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x6f, 0x0e, 0x8a, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x88, 0x8a, 0x8a, 0x4f, 0xe6, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x00, 0xfa, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xae, 0x00, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x6f, 0x0f, 0x18, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8a, 0xef, 0xe6, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0xf0, 0xfe, 0x18, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0x50, 0x0e, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x63, 0xef, 0xe4, 0x8a, 0x88, 0x8a, 0x88, 0x88, 0x88, 0x8a, 0x88, 0x8a, 0x1f, 0x0f, 0x46, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6e, 0xf0, 0x0e, 0x18, 0xa8, 0xa8, 0xa8, 0xa8, 0xa8, 0xaa, 0xee, 0xf0, 0xf6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6e, 0xef, 0x0f, 0xee, 0x41, 0x11, 0x14, 0x4f, 0xef, 0xef, 0xe6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x63, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x56, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 
0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x5e, 0xef, 0xe5, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66
};

static const unsigned long palette_pacman4BPP_UNCOMP[]=
{
	0x000000, 	0xa86e24, 	0xbe8d41, 	0x878787, 
	0x4b2a00, 	0x494a4c, 	0xffffff, 	0xf4bb00, 
	0xe98e02, 	0xf7bb01, 	0xe58c14, 	0x040500, 
	0xf2c300, 	0xfad300, 	0x1f0400, 	0x050d00
};

const tImage  pacman4BPP_UNCOMP=
{
	IMAGE_FMT_4BPP_UNCOMP,
	50,
	50,
	16,
	palette_pacman4BPP_UNCOMP,
	pixel_pacman4BPP_UNCOMP,
};

