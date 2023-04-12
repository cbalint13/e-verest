/*
 *
 *  Copyright (C) 2021  Cristian Balint <cristian dot balint at gmail dot com>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#define STR1(x) #x
#define STR(x) STR1(x)
#define EXTRACT(a, size, offset) (((~(~0 << size) << offset) & a) >> offset)

#define CUSTOMX_OPCODE(x) CUSTOM_##x
#define CUSTOM_0 0b0001011
#define CUSTOM_1 0b0101011
#define CUSTOM_2 0b1011011
#define CUSTOM_3 0b1111011

#define CUSTOMX(X, rd, rs1, rs2, fn7) \
  CUSTOMX_OPCODE(X)                   | \
  (rd                   << (7))       | \
  (0x7                  << (7+5))     | \
  (rs1                  << (7+5+3))   | \
  (rs2                  << (7+5+3+5)) | \
  (EXTRACT(fn7, 7, 0) << (7+5+3+5+5))

/*
 *
 * X   [6:0] = opcode
 * FN3 [2:0] = immediate (set to 0x7)
 * RD  [4:0] = destination register
 * RS1 [4:0] = 1'st source register
 * RS2 [4:0] = 2'nd source register
 * FN7 [6:0] = immediate
 *
 *
 * |31-----25|24---20|19---15|14-12|11---7|6------0|
 * |___FN7___|__RS2__|__RS1__|_FN3_|__RD__|___X____|
 *
 */

#define CUSTOMX_R_R_R(X, rd, rs1, rs2, fn7) \
  asm (".word " STR(CUSTOMX(X, rd, rs1, rs2, fn7)))
