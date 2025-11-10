#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "line.h"
#include "term.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <file>\n", prog);
    fprintf(stderr, "  %s --number <file>\n", prog);
    fprintf(stderr, "  %s --demo\n", prog);
}

static int cmdPreview(const char *path) {
    size_t len = 0;
    char *data = readFile(path, &len);
    if (!data) {
        fprintf(stderr, "error: failed to read '%s'\n", path);
        return 1;
    }
    size_t preview = len < 200 ? len : 200;
    fwrite(data, 1, preview, stdout);
    if (len > preview) fputs("\n...\n", stdout);
    free(data);
    return 0;
}

static int cmdNumber(const char *path) {
    size_t len = 0;
    char *data = readFile(path, &len);
    if (!data) {
        fprintf(stderr, "error: failed to read '%s'\n", path);
        return 1;
    }

    LineVec lv = lineSplit(data, len);
    free(data);

    for (size_t i = 0; i < lv.count; ++i) {
        printf("%zu: %s\n", i + 1, lv.lines[i]);
    }

    lineFree(&lv);
    return 0;
}

static void drawFrame(int row, int col, int max_r, int max_c) {
    term_hide_cursor();
    term_clear_screen();

    term_move_cursor(1, 1);
    printf("test — use arrows to move, q or Ctrl-Q to quit");

    if (row < 2) row = 2;
    if (col < 1) col = 1;
    if (row > max_r) row = max_r;
    if (col > max_c) col = max_c;
    term_move_cursor(row, col);
    putchar('@');

    term_move_cursor(max_r, 1);
    term_show_cursor();
    fflush(stdout);
}

static int cmdDemo(void) {
    if (term_enable_raw() != 0) {
        fprintf(stderr, "error: failed to enable raw mode\n");
        return 1;
    }

    TermSize ts = term_get_size();
    if (ts.rows <= 0 || ts.cols <= 0) {
        ts.rows = 24; ts.cols = 80;
    }

    int r = 3, c = 5;
    drawFrame(r, c, ts.rows, ts.cols);

    for (;;) {
        int ch = 0;
        TermKey k = term_read_key(&ch);
        switch (k) {
            case TERM_KEY_CTRL_Q:
                term_disable_raw();
                term_clear_screen();
                return 0;
            case TERM_KEY_ESC:
                break;
            case TERM_KEY_UP:    r -= 1; break;
            case TERM_KEY_DOWN:  r += 1; break;
            case TERM_KEY_LEFT:  c -= 1; break;
            case TERM_KEY_RIGHT: c += 1; break;
            case TERM_KEY_HOME:  c = 1;  break;
            case TERM_KEY_END:   c = ts.cols; break;
            case TERM_KEY_PAGE_UP:   r = 2; break;
            case TERM_KEY_PAGE_DOWN: r = ts.rows; break;
            case TERM_KEY_CHAR:
                if (ch == 'q') { term_disable_raw(); term_clear_screen(); return 0; }
                break;
            default: break;
        }
        drawFrame(r, c, ts.rows, ts.cols);
    }
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--demo") == 0) {
        return cmdDemo();
    } else if (argc == 2) {
        return cmdPreview(argv[1]);
    } else if (argc == 3 && strcmp(argv[1], "--number") == 0) {
        return cmdNumber(argv[2]);
    } else {
        usage(argv[0]);
        return 1;
    }
}