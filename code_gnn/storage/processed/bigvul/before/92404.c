int read_one_char(FILE *f, char *ret, usec_t t, bool *need_nl) {
        _cleanup_free_ char *line = NULL;
        struct termios old_termios;
        int r;

        assert(f);
        assert(ret);

         
        if (tcgetattr(fileno(f), &old_termios) >= 0) {
                struct termios new_termios = old_termios;

                new_termios.c_lflag &= ~ICANON;
                new_termios.c_cc[VMIN] = 1;
                new_termios.c_cc[VTIME] = 0;

                if (tcsetattr(fileno(f), TCSADRAIN, &new_termios) >= 0) {
                        char c;

                        if (t != USEC_INFINITY) {
                                if (fd_wait_for_event(fileno(f), POLLIN, t) <= 0) {
                                        (void) tcsetattr(fileno(f), TCSADRAIN, &old_termios);
                                        return -ETIMEDOUT;
                                }
                        }

                        r = safe_fgetc(f, &c);
                        (void) tcsetattr(fileno(f), TCSADRAIN, &old_termios);
                        if (r < 0)
                                return r;
                        if (r == 0)
                                return -EIO;

                        if (need_nl)
                                *need_nl = c != '\n';

                        *ret = c;
                        return 0;
                }
        }

        if (t != USEC_INFINITY) {
                if (fd_wait_for_event(fileno(f), POLLIN, t) <= 0)
                        return -ETIMEDOUT;
        }

         

        r = read_line(f, 16, &line);  
        if (r < 0)
                return r;
        if (r == 0)
                return -EIO;

        if (strlen(line) != 1)
                return -EBADMSG;

        if (need_nl)
                *need_nl = false;

        *ret = line[0];
        return 0;
}