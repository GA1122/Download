static int setup_console(const struct lxc_rootfs *rootfs,
			 const struct lxc_console *console,
			 char *ttydir)
{
	 
	if (!rootfs->path)
		return 0;
	if (!ttydir)
		return setup_dev_console(rootfs, console);

	return setup_ttydir_console(rootfs, console, ttydir);
}