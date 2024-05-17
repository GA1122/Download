int tty_release(struct inode *inode, struct file *filp)
{
	struct tty_struct *tty = file_tty(filp);
	struct tty_struct *o_tty;
	int	pty_master, tty_closing, o_tty_closing, do_sleep;
	int	devpts;
	int	idx;
	char	buf[64];

	if (tty_paranoia_check(tty, inode, "tty_release_dev"))
		return 0;

	tty_lock();
	check_tty_count(tty, "tty_release_dev");

	__tty_fasync(-1, filp, 0);

	idx = tty->index;
	pty_master = (tty->driver->type == TTY_DRIVER_TYPE_PTY &&
		      tty->driver->subtype == PTY_TYPE_MASTER);
	devpts = (tty->driver->flags & TTY_DRIVER_DEVPTS_MEM) != 0;
	o_tty = tty->link;

#ifdef TTY_PARANOIA_CHECK
	if (idx < 0 || idx >= tty->driver->num) {
		printk(KERN_DEBUG "tty_release_dev: bad idx when trying to "
				  "free (%s)\n", tty->name);
		tty_unlock();
		return 0;
	}
	if (!devpts) {
		if (tty != tty->driver->ttys[idx]) {
			tty_unlock();
			printk(KERN_DEBUG "tty_release_dev: driver.table[%d] not tty "
			       "for (%s)\n", idx, tty->name);
			return 0;
		}
		if (tty->termios != tty->driver->termios[idx]) {
			tty_unlock();
			printk(KERN_DEBUG "tty_release_dev: driver.termios[%d] not termios "
			       "for (%s)\n",
			       idx, tty->name);
			return 0;
		}
	}
#endif

#ifdef TTY_DEBUG_HANGUP
	printk(KERN_DEBUG "tty_release_dev of %s (tty count=%d)...",
	       tty_name(tty, buf), tty->count);
#endif

#ifdef TTY_PARANOIA_CHECK
	if (tty->driver->other &&
	     !(tty->driver->flags & TTY_DRIVER_DEVPTS_MEM)) {
		if (o_tty != tty->driver->other->ttys[idx]) {
			tty_unlock();
			printk(KERN_DEBUG "tty_release_dev: other->table[%d] "
					  "not o_tty for (%s)\n",
			       idx, tty->name);
			return 0 ;
		}
		if (o_tty->termios != tty->driver->other->termios[idx]) {
			tty_unlock();
			printk(KERN_DEBUG "tty_release_dev: other->termios[%d] "
					  "not o_termios for (%s)\n",
			       idx, tty->name);
			return 0;
		}
		if (o_tty->link != tty) {
			tty_unlock();
			printk(KERN_DEBUG "tty_release_dev: bad pty pointers\n");
			return 0;
		}
	}
#endif
	if (tty->ops->close)
		tty->ops->close(tty, filp);

	tty_unlock();
	 
	while (1) {
		 

		mutex_lock(&tty_mutex);
		tty_lock();
		tty_closing = tty->count <= 1;
		o_tty_closing = o_tty &&
			(o_tty->count <= (pty_master ? 1 : 0));
		do_sleep = 0;

		if (tty_closing) {
			if (waitqueue_active(&tty->read_wait)) {
				wake_up_poll(&tty->read_wait, POLLIN);
				do_sleep++;
			}
			if (waitqueue_active(&tty->write_wait)) {
				wake_up_poll(&tty->write_wait, POLLOUT);
				do_sleep++;
			}
		}
		if (o_tty_closing) {
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

		printk(KERN_WARNING "tty_release_dev: %s: read/write wait queue "
				    "active!\n", tty_name(tty, buf));
		tty_unlock();
		mutex_unlock(&tty_mutex);
		schedule();
	}

	 
	if (pty_master) {
		if (--o_tty->count < 0) {
			printk(KERN_WARNING "tty_release_dev: bad pty slave count "
					    "(%d) for %s\n",
			       o_tty->count, tty_name(o_tty, buf));
			o_tty->count = 0;
		}
	}
	if (--tty->count < 0) {
		printk(KERN_WARNING "tty_release_dev: bad tty->count (%d) for %s\n",
		       tty->count, tty_name(tty, buf));
		tty->count = 0;
	}

	 
	tty_del_file(filp);

	 
	if (tty_closing)
		set_bit(TTY_CLOSING, &tty->flags);
	if (o_tty_closing)
		set_bit(TTY_CLOSING, &o_tty->flags);

	 
	if (tty_closing || o_tty_closing) {
		read_lock(&tasklist_lock);
		session_clear_tty(tty->session);
		if (o_tty)
			session_clear_tty(o_tty->session);
		read_unlock(&tasklist_lock);
	}

	mutex_unlock(&tty_mutex);

	 
	if (!tty_closing || (o_tty && !o_tty_closing)) {
		tty_unlock();
		return 0;
	}

#ifdef TTY_DEBUG_HANGUP
	printk(KERN_DEBUG "freeing tty structure...");
#endif
	 
	tty_ldisc_release(tty, o_tty);
	 
	release_tty(tty, idx);

	 
	if (devpts)
		devpts_kill_index(inode, idx);
	tty_unlock();
	return 0;
}
