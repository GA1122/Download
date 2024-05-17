int tty_release(struct inode *inode, struct file *filp)
{
	struct tty_struct *tty = file_tty(filp);
	struct tty_struct *o_tty = NULL;
	int	do_sleep, final;
	int	idx;
	long	timeout = 0;
	int	once = 1;

	if (tty_paranoia_check(tty, inode, __func__))
		return 0;

	tty_lock(tty);
	check_tty_count(tty, __func__);

	__tty_fasync(-1, filp, 0);

	idx = tty->index;
	if (tty->driver->type == TTY_DRIVER_TYPE_PTY &&
	    tty->driver->subtype == PTY_TYPE_MASTER)
		o_tty = tty->link;

	if (tty_release_checks(tty, idx)) {
		tty_unlock(tty);
		return 0;
	}

	tty_debug_hangup(tty, "releasing (count=%d)\n", tty->count);

	if (tty->ops->close)
		tty->ops->close(tty, filp);

	 
	tty_lock_slave(o_tty);

	 
	while (1) {
		do_sleep = 0;

		if (tty->count <= 1) {
			if (waitqueue_active(&tty->read_wait)) {
				wake_up_poll(&tty->read_wait, POLLIN);
				do_sleep++;
			}
			if (waitqueue_active(&tty->write_wait)) {
				wake_up_poll(&tty->write_wait, POLLOUT);
				do_sleep++;
			}
		}
		if (o_tty && o_tty->count <= 1) {
			if (waitqueue_active(&o_tty->read_wait)) {
				wake_up_poll(&o_tty->read_wait, POLLIN);
				do_sleep++;
			}
			if (waitqueue_active(&o_tty->write_wait)) {
				wake_up_poll(&o_tty->write_wait, POLLOUT);
				do_sleep++;
			}
		}
		if (!do_sleep)
			break;

		if (once) {
			once = 0;
			tty_warn(tty, "read/write wait queue active!\n");
		}
		schedule_timeout_killable(timeout);
		if (timeout < 120 * HZ)
			timeout = 2 * timeout + 1;
		else
			timeout = MAX_SCHEDULE_TIMEOUT;
	}

	if (o_tty) {
		if (--o_tty->count < 0) {
			tty_warn(tty, "bad slave count (%d)\n", o_tty->count);
			o_tty->count = 0;
		}
	}
	if (--tty->count < 0) {
		tty_warn(tty, "bad tty->count (%d)\n", tty->count);
		tty->count = 0;
	}

	 
	tty_del_file(filp);

	 
	if (!tty->count) {
		read_lock(&tasklist_lock);
		session_clear_tty(tty->session);
		if (o_tty)
			session_clear_tty(o_tty->session);
		read_unlock(&tasklist_lock);
	}

	 
	final = !tty->count && !(o_tty && o_tty->count);

	tty_unlock_slave(o_tty);
	tty_unlock(tty);

	 

	if (!final)
		return 0;

	tty_debug_hangup(tty, "final close\n");
	 
	tty_ldisc_release(tty);

	 
	tty_flush_works(tty);

	tty_debug_hangup(tty, "freeing structure\n");
	 
	mutex_lock(&tty_mutex);
	release_tty(tty, idx);
	mutex_unlock(&tty_mutex);

	return 0;
}
