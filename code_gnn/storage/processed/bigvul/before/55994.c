static void tty_ldisc_kill(struct tty_struct *tty)
{
	 
	tty_ldisc_close(tty, tty->ldisc);
	tty_ldisc_put(tty->ldisc);
	 
	tty->ldisc = NULL;

	 
	tty_set_termios_ldisc(tty, N_TTY);
}
