void __tty_hangup(struct tty_struct *tty)
{
	struct file *cons_filp = NULL;
	struct file *filp, *f = NULL;
	struct task_struct *p;
	struct tty_file_private *priv;
	int    closecount = 0, n;
	unsigned long flags;
	int refs = 0;

	if (!tty)
		return;


	spin_lock(&redirect_lock);
	if (redirect && file_tty(redirect) == tty) {
		f = redirect;
		redirect = NULL;
	}
	spin_unlock(&redirect_lock);

	tty_lock();

	 
	set_bit(TTY_HUPPING, &tty->flags);

	 
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

	 
	tty_ldisc_hangup(tty);

	read_lock(&tasklist_lock);
	if (tty->session) {
		do_each_pid_task(tty->session, PIDTYPE_SID, p) {
			spin_lock_irq(&p->sighand->siglock);
			if (p->signal->tty == tty) {
				p->signal->tty = NULL;
				 
				refs++;
			}
			if (!p->signal->leader) {
				spin_unlock_irq(&p->sighand->siglock);
				continue;
			}
			__group_send_sig_info(SIGHUP, SEND_SIG_PRIV, p);
			__group_send_sig_info(SIGCONT, SEND_SIG_PRIV, p);
			put_pid(p->signal->tty_old_pgrp);   
			spin_lock_irqsave(&tty->ctrl_lock, flags);
			if (tty->pgrp)
				p->signal->tty_old_pgrp = get_pid(tty->pgrp);
			spin_unlock_irqrestore(&tty->ctrl_lock, flags);
			spin_unlock_irq(&p->sighand->siglock);
		} while_each_pid_task(tty->session, PIDTYPE_SID, p);
	}
	read_unlock(&tasklist_lock);

	spin_lock_irqsave(&tty->ctrl_lock, flags);
	clear_bit(TTY_THROTTLED, &tty->flags);
	clear_bit(TTY_PUSH, &tty->flags);
	clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
	put_pid(tty->session);
	put_pid(tty->pgrp);
	tty->session = NULL;
	tty->pgrp = NULL;
	tty->ctrl_status = 0;
	spin_unlock_irqrestore(&tty->ctrl_lock, flags);

	 
	while (refs--)
		tty_kref_put(tty);

	 
	if (cons_filp) {
		if (tty->ops->close)
			for (n = 0; n < closecount; n++)
				tty->ops->close(tty, cons_filp);
	} else if (tty->ops->hangup)
		(tty->ops->hangup)(tty);
	 
	set_bit(TTY_HUPPED, &tty->flags);
	clear_bit(TTY_HUPPING, &tty->flags);
	tty_ldisc_enable(tty);

	tty_unlock();

	if (f)
		fput(f);
}
