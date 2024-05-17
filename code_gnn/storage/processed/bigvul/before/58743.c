static int tiocsctty(struct tty_struct *tty, int arg)
{
	int ret = 0;
	if (current->signal->leader && (task_session(current) == tty->session))
		return ret;

	mutex_lock(&tty_mutex);
	 
	if (!current->signal->leader || current->signal->tty) {
		ret = -EPERM;
		goto unlock;
	}

	if (tty->session) {
		 
		if (arg == 1 && capable(CAP_SYS_ADMIN)) {
			 
			read_lock(&tasklist_lock);
			session_clear_tty(tty->session);
			read_unlock(&tasklist_lock);
		} else {
			ret = -EPERM;
			goto unlock;
		}
	}
	proc_set_tty(current, tty);
unlock:
	mutex_unlock(&tty_mutex);
	return ret;
}
