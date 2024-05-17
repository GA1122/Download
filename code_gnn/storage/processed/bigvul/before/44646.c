static int setup_rootfs(struct lxc_conf *conf)
{
	const struct lxc_rootfs *rootfs = &conf->rootfs;

	if (!rootfs->path) {
		if (mount("", "/", NULL, MS_SLAVE|MS_REC, 0)) {
			SYSERROR("Failed to make / rslave");
			return -1;
		}
		return 0;
	}

	if (access(rootfs->mount, F_OK)) {
		SYSERROR("failed to access to '%s', check it is present",
			 rootfs->mount);
		return -1;
	}

	struct bdev *bdev = bdev_init(conf, rootfs->path, rootfs->mount, rootfs->options);
	if (bdev && bdev->ops->mount(bdev) == 0) {
		bdev_put(bdev);
		DEBUG("mounted '%s' on '%s'", rootfs->path, rootfs->mount);
		return 0;
	}
	if (bdev)
		bdev_put(bdev);
	if (mount_rootfs(rootfs->path, rootfs->mount, rootfs->options)) {
		ERROR("failed to mount rootfs");
		return -1;
	}

	DEBUG("mounted '%s' on '%s'", rootfs->path, rootfs->mount);

	return 0;
}