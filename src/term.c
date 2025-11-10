#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE
#include "term.h"

#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

static struct termios orig_termios;
static int raw_enabled = 0;

static void die_disable_raw(void) {
    if (raw_enabled) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        raw_enabled = 0;
    }
}

int term_enable_raw(void) {
    if (!isatty(STDIN_FILENO)) return -1;
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) return -1;

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return -1;
    raw_enabled = 1;
    atexit(die_disable_raw);
    return 0;
}

void term_disable_raw(void) {
    die_disable_raw();
}

static int write_all(const char *buf, size_t len) {
    while (len > 0) {
        ssize_t written = write(STDOUT_FILENO, buf, len);
        if (written == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        buf += (size_t)written;
        len -= (size_t)written;
    }
    return 0;
}

static void write_str(const char *s) {
    (void)write_all(s, strlen(s));
}

void term_clear_screen(void) {
    write_str("\x1b[2J\x1b[H");
}

void term_hide_cursor(void) {
    write_str("\x1b[?25l");
}

void term_show_cursor(void) {
    write_str("\x1b[?25h");
}

void term_move_cursor(int row, int col) {
    if (row < 1) row = 1;
    if (col < 1) col = 1;
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row, col);
    if (n > 0) {
        (void)write_all(buf, (size_t)n);
    }
}

TermSize term_get_size(void) {
    TermSize ts = {0, 0};
    struct winsize wsz;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsz) == -1 || wsz.ws_col == 0) {
        return ts;
    }
    ts.rows = wsz.ws_row;
    ts.cols = wsz.ws_col;
    return ts;
}

static int read_byte(char *c) {
    ssize_t n = read(STDIN_FILENO, c, 1);
    if (n == -1) {
        if (errno == EAGAIN) return 0;
        return -1;
    }
    return (int)n;
}

static TermKey parse_escape_seq(int first_after_esc) {
    char seq[3] = {0,0,0};
    char c;
    if (first_after_esc == '[') {
        if (read_byte(&c) <= 0) return TERM_KEY_ESC;
        if (c >= '0' && c <= '9') {
            seq[0] = c;
            if (read_byte(&c) <= 0) return TERM_KEY_ESC;
            if (c == '~') {
                switch (seq[0]) {
                    case '1': return TERM_KEY_HOME;
                    case '3': return TERM_KEY_DELETE;
                    case '4': return TERM_KEY_END;
                    case '5': return TERM_KEY_PAGE_UP;
                    case '6': return TERM_KEY_PAGE_DOWN;
                    case '7': return TERM_KEY_HOME;
                    case '8': return TERM_KEY_END;
                }
            }
            return TERM_KEY_ESC;
        } else {
            switch (c) {
                case 'A': return TERM_KEY_UP;
                case 'B': return TERM_KEY_DOWN;
                case 'C': return TERM_KEY_RIGHT;
                case 'D': return TERM_KEY_LEFT;
                case 'H': return TERM_KEY_HOME;
                case 'F': return TERM_KEY_END;
                default:  return TERM_KEY_ESC;
            }
        }
    } else if (first_after_esc == 'O') {
        if (read_byte(&c) <= 0) return TERM_KEY_ESC;
        if (c == 'H') return TERM_KEY_HOME;
        if (c == 'F') return TERM_KEY_END;
        return TERM_KEY_ESC;
    }
    return TERM_KEY_ESC;
}

TermKey term_read_key(int *out_char) {
    if (out_char) *out_char = 0;

    for (;;) {
        char c = 0;
        int r = read_byte(&c);
        if (r == -1) return TERM_KEY_NONE;
        if (r == 0)  continue;

        /* Ctrl + key: 0x00..0x1F */
        if (c == 0x11) return TERM_KEY_CTRL_Q;
        if (c == 0x13) return TERM_KEY_CTRL_S;

        switch ((unsigned char)c) {
            case '\r': return TERM_KEY_ENTER;
            case '\t': return TERM_KEY_TAB;
            case 0x1b: {
                char next = 0;
                int rr = read_byte(&next);
                if (rr <= 0) return TERM_KEY_ESC;
                return parse_escape_seq(next);
            }
            case 0x7f:
                return TERM_KEY_BACKSPACE;
            default:
                if (c >= 32 && c <= 126) {
                    if (out_char) *out_char = (int)c;
                    return TERM_KEY_CHAR;
                }
        }
    }
}