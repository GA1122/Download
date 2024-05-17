static int job_control(struct tty_struct *tty, struct file *file)
{
	 
	 
	 
	if (file->f_op->write == redirected_tty_write ||
	    current->signal->tty != tty)
		return 0;

	spin_lock_irq(&tty->ctrl_lock);
	if (!tty->pgrp)
		printk(KERN_ERR "n_tty_read: no tty->pgrp!\n");
	else if (task_pgrp(current) != tty->pgrp) {
		spin_unlock_irq(&tty->ctrl_lock);
		if (is_ignored(SIGTTIN) || is_current_pgrp_orphaned())
			return -EIO;
		kill_pgrp(task_pgrp(current), SIGTTIN, 1);
		set_thread_flag(TIF_SIGPENDING);
		return -ERESTARTSYS;
	}
	spin_unlock_irq(&tty->ctrl_lock);
	return 0;
}
