#ifndef BAYEDIT_TERM_H
#define BAYEDIT_TERM_H

#include <stddef.h>

/* High-level keys your app cares about. Extend as you need. */
typedef enum {
    TERM_KEY_NONE = 0,
    TERM_KEY_CTRL_Q,
    TERM_KEY_CTRL_S,
    TERM_KEY_ENTER,
    TERM_KEY_BACKSPACE,
    TERM_KEY_TAB,
    TERM_KEY_ESC,
    TERM_KEY_UP,
    TERM_KEY_DOWN,
    TERM_KEY_LEFT,
    TERM_KEY_RIGHT,
    TERM_KEY_HOME,
    TERM_KEY_END,
    TERM_KEY_PAGE_UP,
    TERM_KEY_PAGE_DOWN,
    TERM_KEY_DELETE,
    TERM_KEY_CHAR /* printable ASCII; see out_char */
} TermKey;

typedef struct {
    int rows;
    int cols;
} TermSize;

/* Lifecycle */
int  term_enable_raw(void);          /* returns 0 on success */
void term_disable_raw(void);         /* always safe to call */

/* Screen control (ANSI) */
void term_clear_screen(void);
void term_hide_cursor(void);
void term_show_cursor(void);
void term_move_cursor(int row, int col); /* 1-based positions */

/* Window size */
TermSize term_get_size(void);        /* best-effort; returns 0,0 on failure */

/* Input */
TermKey term_read_key(int *out_char);/* blocks; if TERM_KEY_CHAR, writes ASCII to *out_char */

#endif /* BAYEDIT_TERM_H */
