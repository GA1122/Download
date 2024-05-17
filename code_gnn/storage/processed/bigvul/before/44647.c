static int setup_rootfs_pivot_root(const char *rootfs, const char *pivotdir)
{
	int oldroot = -1, newroot = -1;

	oldroot = open("/", O_DIRECTORY | O_RDONLY);
	if (oldroot < 0) {
		SYSERROR("Error opening old-/ for fchdir");
		return -1;
	}
	newroot = open(rootfs, O_DIRECTORY | O_RDONLY);
	if (newroot < 0) {
		SYSERROR("Error opening new-/ for fchdir");
		goto fail;
	}

	 
	if (fchdir(newroot)) {
		SYSERROR("can't chdir to new rootfs '%s'", rootfs);
		goto fail;
	}

	 
	if (pivot_root(".", ".")) {
		SYSERROR("pivot_root syscall failed");
		goto fail;
	}

	 
	if (fchdir(oldroot) < 0) {
		SYSERROR("Error entering oldroot");
		goto fail;
	}
	if (umount2(".", MNT_DETACH) < 0) {
		SYSERROR("Error detaching old root");
		goto fail;
	}

	if (fchdir(newroot) < 0) {
		SYSERROR("Error re-entering newroot");
		goto fail;
	}

	close(oldroot);
	close(newroot);

	DEBUG("pivot_root syscall to '%s' successful", rootfs);

	return 0;

fail:
	if (oldroot != -1)
		close(oldroot);
	if (newroot != -1)
		close(newroot);
	return -1;
}
