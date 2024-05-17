int tmp_proc_mount(struct lxc_conf *lxc_conf)
{
	int mounted;

	mounted = mount_proc_if_needed(lxc_conf->rootfs.path ? lxc_conf->rootfs.mount : "");
	if (mounted == -1) {
		SYSERROR("failed to mount /proc in the container.");
		 
		if (lxc_conf->rootfs.path)
			return -1;
	} else if (mounted == 1) {
		lxc_conf->tmp_umount_proc = 1;
	}
	return 0;
}