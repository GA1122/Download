static void n_tty_kick_worker(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	 
	if (unlikely(ldata->no_room)) {
		ldata->no_room = 0;

		WARN_RATELIMIT(tty->port->itty == NULL,
				"scheduling with invalid itty\n");
		 
		WARN_RATELIMIT(test_bit(TTY_LDISC_HALTED, &tty->flags),
			       "scheduling buffer work for halted ldisc\n");
		tty_buffer_restart_work(tty->port);
	}
}
