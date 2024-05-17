static void n_tty_check_throttle(struct tty_struct *tty)
{
	struct n_tty_data *ldata = tty->disc_data;

	 
	if (ldata->icanon && ldata->canon_head == ldata->read_tail)
		return;

	while (1) {
		int throttled;
		tty_set_flow_change(tty, TTY_THROTTLE_SAFE);
		if (N_TTY_BUF_SIZE - read_cnt(ldata) >= TTY_THRESHOLD_THROTTLE)
			break;
		throttled = tty_throttle_safe(tty);
		if (!throttled)
			break;
	}
	__tty_set_flow_change(tty, 0);
}
