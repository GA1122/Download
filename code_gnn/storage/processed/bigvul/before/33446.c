int do_syslog(int type, char __user *buf, int len, bool from_file)
{
	unsigned i, j, limit, count;
	int do_clear = 0;
	char c;
	int error;

	error = check_syslog_permissions(type, from_file);
	if (error)
		goto out;

	error = security_syslog(type);
	if (error)
		return error;

	switch (type) {
	case SYSLOG_ACTION_CLOSE:	 
		break;
	case SYSLOG_ACTION_OPEN:	 
		break;
	case SYSLOG_ACTION_READ:	 
		error = -EINVAL;
		if (!buf || len < 0)
			goto out;
		error = 0;
		if (!len)
			goto out;
		if (!access_ok(VERIFY_WRITE, buf, len)) {
			error = -EFAULT;
			goto out;
		}
		error = wait_event_interruptible(log_wait,
							(log_start - log_end));
		if (error)
			goto out;
		i = 0;
		raw_spin_lock_irq(&logbuf_lock);
		while (!error && (log_start != log_end) && i < len) {
			c = LOG_BUF(log_start);
			log_start++;
			raw_spin_unlock_irq(&logbuf_lock);
			error = __put_user(c,buf);
			buf++;
			i++;
			cond_resched();
			raw_spin_lock_irq(&logbuf_lock);
		}
		raw_spin_unlock_irq(&logbuf_lock);
		if (!error)
			error = i;
		break;
	 
	case SYSLOG_ACTION_READ_CLEAR:
		do_clear = 1;
		 
	 
	case SYSLOG_ACTION_READ_ALL:
		error = -EINVAL;
		if (!buf || len < 0)
			goto out;
		error = 0;
		if (!len)
			goto out;
		if (!access_ok(VERIFY_WRITE, buf, len)) {
			error = -EFAULT;
			goto out;
		}
		count = len;
		if (count > log_buf_len)
			count = log_buf_len;
		raw_spin_lock_irq(&logbuf_lock);
		if (count > logged_chars)
			count = logged_chars;
		if (do_clear)
			logged_chars = 0;
		limit = log_end;
		 
		for (i = 0; i < count && !error; i++) {
			j = limit-1-i;
			if (j + log_buf_len < log_end)
				break;
			c = LOG_BUF(j);
			raw_spin_unlock_irq(&logbuf_lock);
			error = __put_user(c,&buf[count-1-i]);
			cond_resched();
			raw_spin_lock_irq(&logbuf_lock);
		}
		raw_spin_unlock_irq(&logbuf_lock);
		if (error)
			break;
		error = i;
		if (i != count) {
			int offset = count-error;
			 
			for (i = 0; i < error; i++) {
				if (__get_user(c,&buf[i+offset]) ||
				    __put_user(c,&buf[i])) {
					error = -EFAULT;
					break;
				}
				cond_resched();
			}
		}
		break;
	 
	case SYSLOG_ACTION_CLEAR:
		logged_chars = 0;
		break;
	 
	case SYSLOG_ACTION_CONSOLE_OFF:
		if (saved_console_loglevel == -1)
			saved_console_loglevel = console_loglevel;
		console_loglevel = minimum_console_loglevel;
		break;
	 
	case SYSLOG_ACTION_CONSOLE_ON:
		if (saved_console_loglevel != -1) {
			console_loglevel = saved_console_loglevel;
			saved_console_loglevel = -1;
		}
		break;
	 
	case SYSLOG_ACTION_CONSOLE_LEVEL:
		error = -EINVAL;
		if (len < 1 || len > 8)
			goto out;
		if (len < minimum_console_loglevel)
			len = minimum_console_loglevel;
		console_loglevel = len;
		 
		saved_console_loglevel = -1;
		error = 0;
		break;
	 
	case SYSLOG_ACTION_SIZE_UNREAD:
		error = log_end - log_start;
		break;
	 
	case SYSLOG_ACTION_SIZE_BUFFER:
		error = log_buf_len;
		break;
	default:
		error = -EINVAL;
		break;
	}
out:
	return error;
}