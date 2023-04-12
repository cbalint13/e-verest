/*
 *  PicoSoC - A simple example SoC using PicoRV32
 *
 *  Copyright (C) 2017  Clifford Wolf <clifford@clifford.at>
 *                2023  Balint Cristian <cristian dot balint at gmail dot com>
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

#include <stdint.h>
#include <stdbool.h>


#define F_CPU     50000000 // Hz
#define BAUD_RATE   115200 // bit/s
#define MEM_TOTAL   0x8000

#define reg_uart_clkdiv (*(volatile uint32_t*)0x02000004)
#define reg_uart_data (*(volatile uint32_t*)0x02000008)

// --------------------------------------------------------

int putchar(int c) {
  c &= 0xFF;
  if (c == '\n') putchar('\r');
  reg_uart_data = c;
}

void print(const char *p) {
  while (*p) putchar(*(p++));
}

void print_hex(uint32_t v, int digits) {
  for (int i = 7; i >= 0; i--) {
    char c = "0123456789abcdef"[(v >> (4*i)) & 15];
    if (c == '0' && i >= digits) continue;
    putchar(c);
    digits = i;
  }
}

void print_dec(uint32_t v) {
  if (v >= 1000) {
    print(">=1000");
    return;
  }

  if      (v >= 900) { putchar('9'); v -= 900; }
  else if (v >= 800) { putchar('8'); v -= 800; }
  else if (v >= 700) { putchar('7'); v -= 700; }
  else if (v >= 600) { putchar('6'); v -= 600; }
  else if (v >= 500) { putchar('5'); v -= 500; }
  else if (v >= 400) { putchar('4'); v -= 400; }
  else if (v >= 300) { putchar('3'); v -= 300; }
  else if (v >= 200) { putchar('2'); v -= 200; }
  else if (v >= 100) { putchar('1'); v -= 100; }

  if      (v >= 90) { putchar('9'); v -= 90; }
  else if (v >= 80) { putchar('8'); v -= 80; }
  else if (v >= 70) { putchar('7'); v -= 70; }
  else if (v >= 60) { putchar('6'); v -= 60; }
  else if (v >= 50) { putchar('5'); v -= 50; }
  else if (v >= 40) { putchar('4'); v -= 40; }
  else if (v >= 30) { putchar('3'); v -= 30; }
  else if (v >= 20) { putchar('2'); v -= 20; }
  else if (v >= 10) { putchar('1'); v -= 10; }

  if      (v >= 9) { putchar('9'); v -= 9; }
  else if (v >= 8) { putchar('8'); v -= 8; }
  else if (v >= 7) { putchar('7'); v -= 7; }
  else if (v >= 6) { putchar('6'); v -= 6; }
  else if (v >= 5) { putchar('5'); v -= 5; }
  else if (v >= 4) { putchar('4'); v -= 4; }
  else if (v >= 3) { putchar('3'); v -= 3; }
  else if (v >= 2) { putchar('2'); v -= 2; }
  else if (v >= 1) { putchar('1'); v -= 1; }
  else putchar('0');
}

char getchar_prompt(char *prompt) {

  if (prompt) {
    print(prompt);
  }

  int32_t c = -1;

  while (c == -1) {
    c = reg_uart_data;
  }

  return c & 0xFF;
}

char getchar() {
  return getchar_prompt(0);
}

// --------------------------------------------------------

void cmd_echo() {
  print("Return to menu by sending '!'\n\n");
  char c;
  while ((c = getchar()) != '!') putchar(c);
}

// --------------------------------------------------------

void main() {
  int c = 0;
  while (c < 10000) { c++; }
  c = 0;

  reg_uart_clkdiv = F_CPU/BAUD_RATE;
  print("Booting..\n");

  //print("Press ENTER to continue..\n");
  // while (getchar_prompt(0) != '\r') { /* wait */ }

  print("\n");

  print("     __     _______ ____  _____ ____ _____\n");
  print("   __\\ \\   / | ____|  _ \\| ____/ ___|_   _|\n");
  print("  / _ \\ \\ / /|  _| | |_) |  _| \\___ \\ | |\n");
  print(" |  __/\\ V / | |___|  _ <| |___ ___) || |\n");
  print("  \\___| \\_/  |_____|_| \\_|_____|____/ |_|\n");

  print("\n");

  print("Total memory: ");
  print_dec(MEM_TOTAL / 1024);
  print(" KiB\n");
  print("\n");

  //cmd_memtest(); // test overwrites bss and data memory
  print("\n");

  while (1)
  {
    print("\n");

    print("Select an action:\n");
    print("\n");
    print("   [e] Echo UART\n");
    print("\n");

    for (int rep = 10; rep > 0; rep--)
    {
      print("Command> ");
      char cmd = getchar();
      if (cmd > 32 && cmd < 127) putchar(cmd);
      print("\n");

      switch (cmd)
      {
      case 'e':
        cmd_echo();
        break;
      default:
        continue;
      }

      break;
    }
  }
}
