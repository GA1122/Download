int reset_terminal_fd(int fd, bool switch_to_text) {
        struct termios termios;
        int r = 0;

         

        assert(fd >= 0);

         

         
        (void) ioctl(fd, TIOCNXCL);

         
        if (switch_to_text)
                (void) ioctl(fd, KDSETMODE, KD_TEXT);

         
        (void) vt_reset_keyboard(fd);

        if (tcgetattr(fd, &termios) < 0) {
                r = -errno;
                goto finish;
        }

         

        termios.c_iflag &= ~(IGNBRK | BRKINT | ISTRIP | INLCR | IGNCR | IUCLC);
        termios.c_iflag |= ICRNL | IMAXBEL | IUTF8;
        termios.c_oflag |= ONLCR;
        termios.c_cflag |= CREAD;
        termios.c_lflag = ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOPRT | ECHOKE;

        termios.c_cc[VINTR]    =   03;   
        termios.c_cc[VQUIT]    =  034;   
        termios.c_cc[VERASE]   = 0177;
        termios.c_cc[VKILL]    =  025;   
        termios.c_cc[VEOF]     =   04;   
        termios.c_cc[VSTART]   =  021;   
        termios.c_cc[VSTOP]    =  023;   
        termios.c_cc[VSUSP]    =  032;   
        termios.c_cc[VLNEXT]   =  026;   
        termios.c_cc[VWERASE]  =  027;   
        termios.c_cc[VREPRINT] =  022;   
        termios.c_cc[VEOL]     =    0;
        termios.c_cc[VEOL2]    =    0;

        termios.c_cc[VTIME]  = 0;
        termios.c_cc[VMIN]   = 1;

        if (tcsetattr(fd, TCSANOW, &termios) < 0)
                r = -errno;

finish:
         
        (void) tcflush(fd, TCIOFLUSH);

        return r;
}
