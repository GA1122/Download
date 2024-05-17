struct tty_struct *tty_init_dev(struct tty_driver *driver, int idx)
{
	struct tty_struct *tty;
	int retval;

	 

	if (!try_module_get(driver->owner))
		return ERR_PTR(-ENODEV);

	tty = alloc_tty_struct(driver, idx);
	if (!tty) {
		retval = -ENOMEM;
		goto err_module_put;
	}

	tty_lock(tty);
	retval = tty_driver_install_tty(driver, tty);
	if (retval < 0)
		goto err_deinit_tty;

	if (!tty->port)
		tty->port = driver->ports[idx];

	WARN_RATELIMIT(!tty->port,
			"%s: %s driver does not set tty->port. This will crash the kernel later. Fix the driver!\n",
			__func__, tty->driver->name);

	tty->port->itty = tty;

	 
	retval = tty_ldisc_setup(tty, tty->link);
	if (retval)
		goto err_release_tty;
	 
	return tty;

err_deinit_tty:
	tty_unlock(tty);
	deinitialize_tty_struct(tty);
	free_tty_struct(tty);
err_module_put:
	module_put(driver->owner);
	return ERR_PTR(retval);

	 
err_release_tty:
	tty_unlock(tty);
	tty_info_ratelimited(tty, "ldisc open failed (%d), clearing slot %d\n",
			     retval, idx);
	release_tty(tty, idx);
	return ERR_PTR(retval);
}
