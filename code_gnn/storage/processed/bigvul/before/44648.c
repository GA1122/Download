static int setup_tty(struct lxc_conf *conf)
{
	const struct lxc_tty_info *tty_info = &conf->tty_info;
	char *ttydir = conf->ttydir;
	char path[MAXPATHLEN], lxcpath[MAXPATHLEN];
	int i, ret;

	if (!conf->rootfs.path)
		return 0;

	for (i = 0; i < tty_info->nbtty; i++) {

		struct lxc_pty_info *pty_info = &tty_info->pty_info[i];

		ret = snprintf(path, sizeof(path), "/dev/tty%d", i + 1);
		if (ret >= sizeof(path)) {
			ERROR("pathname too long for ttys");
			return -1;
		}
		if (ttydir) {
			 
			ret = snprintf(lxcpath, sizeof(lxcpath), "/dev/%s/tty%d", ttydir, i + 1);
			if (ret >= sizeof(lxcpath)) {
				ERROR("pathname too long for ttys");
				return -1;
			}
			ret = creat(lxcpath, 0660);
			if (ret==-1 && errno != EEXIST) {
				SYSERROR("error creating %s", lxcpath);
				return -1;
			}
			if (ret >= 0)
				close(ret);
			ret = unlink(path);
			if (ret && errno != ENOENT) {
				SYSERROR("error unlinking %s", path);
				return -1;
			}

			if (mount(pty_info->name, lxcpath, "none", MS_BIND, 0)) {
				WARN("failed to mount '%s'->'%s'",
				     pty_info->name, path);
				continue;
			}

			ret = snprintf(lxcpath, sizeof(lxcpath), "%s/tty%d", ttydir, i+1);
			if (ret >= sizeof(lxcpath)) {
				ERROR("tty pathname too long");
				return -1;
			}
			ret = symlink(lxcpath, path);
			if (ret) {
				SYSERROR("failed to create symlink for tty %d", i+1);
				return -1;
			}
		} else {
			 
			if (access(path, F_OK)) {
				ret = creat(path, 0660);
				if (ret==-1) {
					SYSERROR("error creating %s", path);
					 
				} else {
					close(ret);
				}
			}
			if (mount(pty_info->name, path, "none", MS_BIND, 0)) {
				SYSERROR("failed to mount '%s'->'%s'", pty_info->name, path);
				continue;
			}
		}
		if (!append_ptyname(&conf->pty_names, pty_info->name)) {
			ERROR("Error setting up container_ttys string");
			return -1;
		}
	}

	INFO("%d tty(s) has been setup", tty_info->nbtty);

	return 0;
}