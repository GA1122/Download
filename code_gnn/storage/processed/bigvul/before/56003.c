void tty_ldisc_release(struct tty_struct *tty)
{
	struct tty_struct *o_tty = tty->link;

	 

	tty_ldisc_lock_pair(tty, o_tty);
	tty_ldisc_kill(tty);
	if (o_tty)
		tty_ldisc_kill(o_tty);
	tty_ldisc_unlock_pair(tty, o_tty);

	 

	tty_ldisc_debug(tty, "released\n");
}
