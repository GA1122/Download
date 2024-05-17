struct tty_struct *tty_init_dev(struct tty_driver *driver, int idx,
								int first_ok)
{
	struct tty_struct *tty;
	int retval;

	 
	if (driver->subtype == PTY_TYPE_MASTER &&
		(driver->flags & TTY_DRIVER_DEVPTS_MEM) && !first_ok) {
		return ERR_PTR(-EIO);
	}

	 

	if (!try_module_get(driver->owner))
		return ERR_PTR(-ENODEV);

	tty = alloc_tty_struct();
	if (!tty) {
		retval = -ENOMEM;
		goto err_module_put;
	}
	initialize_tty_struct(tty, driver, idx);

	retval = tty_driver_install_tty(driver, tty);
	if (retval < 0)
		goto err_deinit_tty;

	 
	retval = tty_ldisc_setup(tty, tty->link);
	if (retval)
		goto err_release_tty;
	return tty;

err_deinit_tty:
	deinitialize_tty_struct(tty);
	free_tty_struct(tty);
err_module_put:
	module_put(driver->owner);
	return ERR_PTR(retval);

	 
err_release_tty:
	printk_ratelimited(KERN_INFO "tty_init_dev: ldisc open failed, "
				 "clearing slot %d\n", idx);
	release_tty(tty, idx);
	return ERR_PTR(retval);
}
