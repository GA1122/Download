void tty_free_termios(struct tty_struct *tty)
{
	struct ktermios *tp;
	int idx = tty->index;
	 
	if (tty->driver->flags & TTY_DRIVER_RESET_TERMIOS) {
		 
		tp = tty->termios;
		tty->driver->termios[idx] = NULL;
		kfree(tp);
	}
}
