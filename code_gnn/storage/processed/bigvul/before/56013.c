int tty_set_ldisc(struct tty_struct *tty, int ldisc)
{
	int retval;
	struct tty_ldisc *old_ldisc, *new_ldisc;

	new_ldisc = tty_ldisc_get(tty, ldisc);
	if (IS_ERR(new_ldisc))
		return PTR_ERR(new_ldisc);

	tty_lock(tty);
	retval = tty_ldisc_lock(tty, 5 * HZ);
	if (retval)
		goto err;

	 
	if (tty->ldisc->ops->num == ldisc)
		goto out;

	if (test_bit(TTY_HUPPED, &tty->flags)) {
		 
		retval = -EIO;
		goto out;
	}

	old_ldisc = tty->ldisc;

	 
	tty_ldisc_close(tty, old_ldisc);

	 
	tty->ldisc = new_ldisc;
	tty_set_termios_ldisc(tty, ldisc);

	retval = tty_ldisc_open(tty, new_ldisc);
	if (retval < 0) {
		 
		tty_ldisc_put(new_ldisc);
		tty_ldisc_restore(tty, old_ldisc);
	}

	if (tty->ldisc->ops->num != old_ldisc->ops->num && tty->ops->set_ldisc) {
		down_read(&tty->termios_rwsem);
		tty->ops->set_ldisc(tty);
		up_read(&tty->termios_rwsem);
	}

	 
	new_ldisc = old_ldisc;
out:
	tty_ldisc_unlock(tty);

	 
	tty_buffer_restart_work(tty->port);
err:
	tty_ldisc_put(new_ldisc);	 
	tty_unlock(tty);
	return retval;
}
