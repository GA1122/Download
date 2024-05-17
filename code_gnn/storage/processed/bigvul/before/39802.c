static void n_tty_check_unthrottle(struct tty_struct *tty)
{
	if (tty->driver->type == TTY_DRIVER_TYPE_PTY &&
	    tty->link->ldisc->ops->write_wakeup == n_tty_write_wakeup) {
		if (chars_in_buffer(tty) > TTY_THRESHOLD_UNTHROTTLE)
			return;
		if (!tty->count)
			return;
		n_tty_set_room(tty);
		n_tty_write_wakeup(tty->link);
		if (waitqueue_active(&tty->link->write_wait))
			wake_up_interruptible_poll(&tty->link->write_wait, POLLOUT);
		return;
	}

	 

	while (1) {
		int unthrottled;
		tty_set_flow_change(tty, TTY_UNTHROTTLE_SAFE);
		if (chars_in_buffer(tty) > TTY_THRESHOLD_UNTHROTTLE)
			break;
		if (!tty->count)
			break;
		n_tty_set_room(tty);
		unthrottled = tty_unthrottle_safe(tty);
		if (!unthrottled)
			break;
	}
	__tty_set_flow_change(tty, 0);
}
