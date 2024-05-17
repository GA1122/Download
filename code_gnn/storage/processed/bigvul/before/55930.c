static int tty_open(struct inode *inode, struct file *filp)
{
	struct tty_struct *tty;
	int noctty, retval;
	struct tty_driver *driver = NULL;
	int index;
	dev_t device = inode->i_rdev;
	unsigned saved_flags = filp->f_flags;

	nonseekable_open(inode, filp);

retry_open:
	retval = tty_alloc_file(filp);
	if (retval)
		return -ENOMEM;

	noctty = filp->f_flags & O_NOCTTY;
	index  = -1;
	retval = 0;

	tty = tty_open_current_tty(device, filp);
	if (!tty) {
		mutex_lock(&tty_mutex);
		driver = tty_lookup_driver(device, filp, &noctty, &index);
		if (IS_ERR(driver)) {
			retval = PTR_ERR(driver);
			goto err_unlock;
		}

		 
		tty = tty_driver_lookup_tty(driver, inode, index);
		if (IS_ERR(tty)) {
			retval = PTR_ERR(tty);
			goto err_unlock;
		}

		if (tty) {
			mutex_unlock(&tty_mutex);
			retval = tty_lock_interruptible(tty);
			if (retval) {
				if (retval == -EINTR)
					retval = -ERESTARTSYS;
				goto err_unref;
			}
			 
			tty_kref_put(tty);
			retval = tty_reopen(tty);
			if (retval < 0) {
				tty_unlock(tty);
				tty = ERR_PTR(retval);
			}
		} else {  
			tty = tty_init_dev(driver, index);
			mutex_unlock(&tty_mutex);
		}

		tty_driver_kref_put(driver);
	}

	if (IS_ERR(tty)) {
		retval = PTR_ERR(tty);
		if (retval != -EAGAIN || signal_pending(current))
			goto err_file;
		tty_free_file(filp);
		schedule();
		goto retry_open;
	}

	tty_add_file(tty, filp);

	check_tty_count(tty, __func__);
	if (tty->driver->type == TTY_DRIVER_TYPE_PTY &&
	    tty->driver->subtype == PTY_TYPE_MASTER)
		noctty = 1;

	tty_debug_hangup(tty, "opening (count=%d)\n", tty->count);

	if (tty->ops->open)
		retval = tty->ops->open(tty, filp);
	else
		retval = -ENODEV;
	filp->f_flags = saved_flags;

	if (retval) {
		tty_debug_hangup(tty, "open error %d, releasing\n", retval);

		tty_unlock(tty);  
		tty_release(inode, filp);
		if (retval != -ERESTARTSYS)
			return retval;

		if (signal_pending(current))
			return retval;

		schedule();
		 
		if (tty_hung_up_p(filp))
			filp->f_op = &tty_fops;
		goto retry_open;
	}
	clear_bit(TTY_HUPPED, &tty->flags);


	read_lock(&tasklist_lock);
	spin_lock_irq(&current->sighand->siglock);
	if (!noctty &&
	    current->signal->leader &&
	    !current->signal->tty &&
	    tty->session == NULL) {
		 
		if (filp->f_mode & FMODE_READ)
			__proc_set_tty(tty);
	}
	spin_unlock_irq(&current->sighand->siglock);
	read_unlock(&tasklist_lock);
	tty_unlock(tty);
	return 0;
err_unlock:
	mutex_unlock(&tty_mutex);
err_unref:
	 
	if (!IS_ERR_OR_NULL(driver))
		tty_driver_kref_put(driver);
err_file:
	tty_free_file(filp);
	return retval;
}
