static void release_tty(struct tty_struct *tty, int idx)
{
	 
	WARN_ON(tty->index != idx);

	if (tty->link)
		tty_kref_put(tty->link);
	tty_kref_put(tty);
}
