static ssize_t n_tty_read(struct tty_struct *tty, struct file *file,
			 unsigned char __user *buf, size_t nr)
{
	struct n_tty_data *ldata = tty->disc_data;
	unsigned char __user *b = buf;
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
	int c;
	int minimum, time;
	ssize_t retval = 0;
	long timeout;
	int packet;
	size_t tail;

	c = job_control(tty, file);
	if (c < 0)
		return c;

	 
	if (file->f_flags & O_NONBLOCK) {
		if (!mutex_trylock(&ldata->atomic_read_lock))
			return -EAGAIN;
	} else {
		if (mutex_lock_interruptible(&ldata->atomic_read_lock))
			return -ERESTARTSYS;
	}

	down_read(&tty->termios_rwsem);

	minimum = time = 0;
	timeout = MAX_SCHEDULE_TIMEOUT;
	if (!ldata->icanon) {
		minimum = MIN_CHAR(tty);
		if (minimum) {
			time = (HZ / 10) * TIME_CHAR(tty);
		} else {
			timeout = (HZ / 10) * TIME_CHAR(tty);
			minimum = 1;
		}
	}

	packet = tty->packet;
	tail = ldata->read_tail;

	add_wait_queue(&tty->read_wait, &wait);
	while (nr) {
		 
		if (packet && tty->link->ctrl_status) {
			unsigned char cs;
			if (b != buf)
				break;
			spin_lock_irq(&tty->link->ctrl_lock);
			cs = tty->link->ctrl_status;
			tty->link->ctrl_status = 0;
			spin_unlock_irq(&tty->link->ctrl_lock);
			if (put_user(cs, b)) {
				retval = -EFAULT;
				break;
			}
			b++;
			nr--;
			break;
		}

		if (!input_available_p(tty, 0)) {
			up_read(&tty->termios_rwsem);
			tty_buffer_flush_work(tty->port);
			down_read(&tty->termios_rwsem);
			if (!input_available_p(tty, 0)) {
				if (test_bit(TTY_OTHER_CLOSED, &tty->flags)) {
					retval = -EIO;
					break;
				}
				if (tty_hung_up_p(file))
					break;
				if (!timeout)
					break;
				if (file->f_flags & O_NONBLOCK) {
					retval = -EAGAIN;
					break;
				}
				if (signal_pending(current)) {
					retval = -ERESTARTSYS;
					break;
				}
				up_read(&tty->termios_rwsem);

				timeout = wait_woken(&wait, TASK_INTERRUPTIBLE,
						timeout);

				down_read(&tty->termios_rwsem);
				continue;
			}
		}

		if (ldata->icanon && !L_EXTPROC(tty)) {
			retval = canon_copy_from_read_buf(tty, &b, &nr);
			if (retval)
				break;
		} else {
			int uncopied;

			 
			if (packet && b == buf) {
				if (put_user(TIOCPKT_DATA, b)) {
					retval = -EFAULT;
					break;
				}
				b++;
				nr--;
			}

			uncopied = copy_from_read_buf(tty, &b, &nr);
			uncopied += copy_from_read_buf(tty, &b, &nr);
			if (uncopied) {
				retval = -EFAULT;
				break;
			}
		}

		n_tty_check_unthrottle(tty);

		if (b - buf >= minimum)
			break;
		if (time)
			timeout = time;
	}
	if (tail != ldata->read_tail)
		n_tty_kick_worker(tty);
	up_read(&tty->termios_rwsem);

	remove_wait_queue(&tty->read_wait, &wait);
	mutex_unlock(&ldata->atomic_read_lock);

	if (b - buf)
		retval = b - buf;

	return retval;
}