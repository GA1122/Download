 static int mount_file_entries(const struct lxc_rootfs *rootfs, FILE *file,
	const char *lxc_name)
{
	struct mntent mntent;
	char buf[4096];
	int ret = -1;

	while (getmntent_r(file, &mntent, buf, sizeof(buf))) {

		if (!rootfs->path) {
			if (mount_entry_on_systemfs(&mntent))
				goto out;
			continue;
		}

		 
		if (mntent.mnt_dir[0] != '/') {
			if (mount_entry_on_relative_rootfs(&mntent,
							   rootfs->mount))
				goto out;
			continue;
		}

		if (mount_entry_on_absolute_rootfs(&mntent, rootfs, lxc_name))
			goto out;
	}

	ret = 0;

	INFO("mount points have been setup");
out:
	return ret;
}