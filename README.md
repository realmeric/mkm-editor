# mkm-editor

## How to compile and run:
Simply run `make`

## First Test - File Read / Writing Function
Ran
```bash
make
echo -e "alpha\nbeta\r\ngamma" > read.txt
./build/mkm read.txt
```
and it gave me first the 200 bytes of the file sucessfully.

`malloc` allocates memory and returns a pointer to it.
On `calloc` the memory is not initialized.
`fseek` used for setting the file pointer at the specific position in the file.
`ftell` used to obtain the current file position of a stream.

## Second Test - Counting Lines
Added `--number` flag for mkm, which displays the number of lines in the given file.

```bash
printf "file has one line" > ol.txt
./build/mkm --number ol.txt
```
Expected output: `1: file has one line`
```bash
printf "\n" > twoblanklines.txt
./build/mkm --number twoblanklines.txt
```
Expected output: 
```bash
1: 
2: 
```

Combined 13 decimal (0x0D) and 10 decimal (0x0A) which is CR and LF, they are used to mark a line break in a text file.

## Third Test - Terminal I/O Function

```bash
./build/mkm --demo
```
to enter on demo.

- BRKINT: disable SIGINT on break condition.
- ICRNL: don't translate CR to NL.
- INPCK: parity checking off.
- ISTRIP: don't strip 8th bit (keep bytes intact).
- IXON: disable XON/XOFF software flow control.
- OPOST: disable post-processing; "\n" will NOT become "\r\n".
- CS8: set 8-bit chars.
- ECHO: stop echoing typed keys.
- ICANON: non-canonical mode (no line buffering).
- IEXTEN: turn off implementation-defined special controls (e.g., Ctrl-V).
- ISIG: turn off signals from INTR, QUIT, SUSP.

### ANSI Escape Codes Cheat Sheet

| Sequence | Description | Notes / Example |
| --- | --- | --- |
| `\x1b[2J` | Clear entire screen | Often followed by `\x1b[H` to move cursor home. |
| `\x1b[H` | Move cursor to row 1, column 1 | Use `\x1b[row;colH` for specific coordinates. |
| `\x1b[s` / `\x1b[u` | Save / restore cursor position | Restores last saved spot. |
| `\x1b[nA` | Move cursor up `n` rows | `B` = down, `C` = right, `D` = left. |
| `\x1b[?25l` / `\x1b[?25h` | Hide / show cursor | Useful for full-screen UIs. |
| `\x1b[K` | Clear to end of line | `\x1b[1K` clears to start; `\x1b[2K` entire line. |
| `\x1b[0m` | Reset all attributes | Always reset after styling. |
| `\x1b[1m` | Bold / bright text | `\x1b[2m` dim, `\x1b[3m` italic, `\x1b[4m` underline. |
| `\x1b[30–37m` | Set foreground color (basic) | 30 black … 37 white; add 60 for bright variants (`90–97`). |
| `\x1b[40–47m` | Set background color (basic) | 40–47 normal, 100–107 bright. |
| `\x1b[38;5;Nm` | Set 256-color foreground | `N` 0–255 palette; background uses `48;5;N`. |
| `\x1b[38;2;R;G;Bm` | Truecolor foreground | Background `48;2;R;G;B`. |
| `\x1b]0;title\x07` | Set terminal window title | BEL (`\x07`) terminates the OSC sequence. |
| `\x1b[?1049h` / `\x1b[?1049l` | Enter / exit alternate screen buffer | Keeps main screen intact while full-screen UI runs. |

> Prefix every sequence with the escape byte (`\x1b` or `\033`); wrap styled text like `\x1b[1;31mHello\x1b[0m`.