static int lxc_attach_remount_sys_proc(void)
{
	int ret;

	ret = unshare(CLONE_NEWNS);
	if (ret < 0) {
		SYSERROR("failed to unshare mount namespace");
		return -1;
	}

	if (detect_shared_rootfs()) {
		if (mount(NULL, "/", NULL, MS_SLAVE|MS_REC, NULL)) {
			SYSERROR("Failed to make / rslave");
			ERROR("Continuing...");
		}
	}

	 
	ret = umount2("/proc", MNT_DETACH);
	if (ret < 0) {
		SYSERROR("failed to unmount /proc");
		return -1;
	}

	ret = mount("none", "/proc", "proc", 0, NULL);
	if (ret < 0) {
		SYSERROR("failed to remount /proc");
		return -1;
	}

	 
	ret = umount2("/sys", MNT_DETACH);
	if (ret < 0 && errno != EINVAL) {
		SYSERROR("failed to unmount /sys");
		return -1;
	} else if (ret == 0) {
		 
		ret = mount("none", "/sys", "sysfs", 0, NULL);
		if (ret < 0) {
			SYSERROR("failed to remount /sys");
			return -1;
		}
	}

	return 0;
}
