static void __tty_hangup(struct tty_struct *tty, int exit_session)
{
	struct file *cons_filp = NULL;
	struct file *filp, *f = NULL;
	struct tty_file_private *priv;
	int    closecount = 0, n;
	int refs;

	if (!tty)
		return;


	spin_lock(&redirect_lock);
	if (redirect && file_tty(redirect) == tty) {
		f = redirect;
		redirect = NULL;
	}
	spin_unlock(&redirect_lock);

	tty_lock(tty);

	if (test_bit(TTY_HUPPED, &tty->flags)) {
		tty_unlock(tty);
		return;
	}

	 
	check_tty_count(tty, "tty_hangup");

	spin_lock(&tty_files_lock);
	 
	list_for_each_entry(priv, &tty->tty_files, list) {
		filp = priv->file;
		if (filp->f_op->write == redirected_tty_write)
			cons_filp = filp;
		if (filp->f_op->write != tty_write)
			continue;
		closecount++;
		__tty_fasync(-1, filp, 0);	 
		filp->f_op = &hung_up_tty_fops;
	}
	spin_unlock(&tty_files_lock);

	refs = tty_signal_session_leader(tty, exit_session);
	 
	while (refs--)
		tty_kref_put(tty);

	tty_ldisc_hangup(tty);

	spin_lock_irq(&tty->ctrl_lock);
	clear_bit(TTY_THROTTLED, &tty->flags);
	clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
	put_pid(tty->session);
	put_pid(tty->pgrp);
	tty->session = NULL;
	tty->pgrp = NULL;
	tty->ctrl_status = 0;
	spin_unlock_irq(&tty->ctrl_lock);

	 
	if (cons_filp) {
		if (tty->ops->close)
			for (n = 0; n < closecount; n++)
				tty->ops->close(tty, cons_filp);
	} else if (tty->ops->hangup)
		tty->ops->hangup(tty);
	 
	set_bit(TTY_HUPPED, &tty->flags);
	tty_unlock(tty);

	if (f)
		fput(f);
}