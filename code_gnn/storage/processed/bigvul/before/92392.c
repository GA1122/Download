void get_log_colors(int priority, const char **on, const char **off, const char **highlight) {
         

        if (priority <= LOG_ERR) {
                if (on)
                        *on = ANSI_HIGHLIGHT_RED;
                if (off)
                        *off = ANSI_NORMAL;
                if (highlight)
                        *highlight = ANSI_HIGHLIGHT;

        } else if (priority <= LOG_WARNING) {
                if (on)
                        *on = ANSI_HIGHLIGHT_YELLOW;
                if (off)
                        *off = ANSI_NORMAL;
                if (highlight)
                        *highlight = ANSI_HIGHLIGHT;

        } else if (priority <= LOG_NOTICE) {
                if (on)
                        *on = ANSI_HIGHLIGHT;
                if (off)
                        *off = ANSI_NORMAL;
                if (highlight)
                        *highlight = ANSI_HIGHLIGHT_RED;

        } else if (priority >= LOG_DEBUG) {
                if (on)
                        *on = ANSI_GREY;
                if (off)
                        *off = ANSI_NORMAL;
                if (highlight)
                        *highlight = ANSI_HIGHLIGHT_RED;
        }
}
