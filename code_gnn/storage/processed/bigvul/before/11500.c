int ask(char *ret, const char *replies, const char *text, ...) {
        bool on_tty;

        assert(ret);
        assert(replies);
        assert(text);

        on_tty = isatty(STDOUT_FILENO);

        for (;;) {
                va_list ap;
                char c;
                int r;
                bool need_nl = true;

                if (on_tty)
                        fputs(ANSI_HIGHLIGHT_ON, stdout);

                va_start(ap, text);
                vprintf(text, ap);
                va_end(ap);

                if (on_tty)
                        fputs(ANSI_HIGHLIGHT_OFF, stdout);

                fflush(stdout);

                r = read_one_char(stdin, &c, (usec_t) -1, &need_nl);
                if (r < 0) {

                        if (r == -EBADMSG) {
                                puts("Bad input, please try again.");
                                continue;
                        }

                        putchar('\n');
                        return r;
                }

                if (need_nl)
                        putchar('\n');

                if (strchr(replies, c)) {
                        *ret = c;
                        return 0;
                }

                puts("Read unexpected character, please try again.");
        }
}
