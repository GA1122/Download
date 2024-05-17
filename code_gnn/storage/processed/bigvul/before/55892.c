static int tiocsctty(struct tty_struct *tty, struct file *file, int arg)
{
	int ret = 0;

	tty_lock(tty);
	read_lock(&tasklist_lock);

	if (current->signal->leader && (task_session(current) == tty->session))
		goto unlock;

	 
	if (!current->signal->leader || current->signal->tty) {
		ret = -EPERM;
		goto unlock;
	}

	if (tty->session) {
		 
		if (arg == 1 && capable(CAP_SYS_ADMIN)) {
			 
			session_clear_tty(tty->session);
		} else {
			ret = -EPERM;
			goto unlock;
		}
	}

	 
	if ((file->f_mode & FMODE_READ) == 0 && !capable(CAP_SYS_ADMIN)) {
		ret = -EPERM;
		goto unlock;
	}

	proc_set_tty(tty);
unlock:
	read_unlock(&tasklist_lock);
	tty_unlock(tty);
	return ret;
}
