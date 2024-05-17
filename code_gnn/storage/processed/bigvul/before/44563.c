static int find_fstype_cb(char* buffer, void *data)
{
	struct cbarg {
		const char *rootfs;
		const char *target;
		const char *options;
	} *cbarg = data;

	unsigned long mntflags;
	char *mntdata;
	char *fstype;

	 
	if (strstr(buffer, "nodev"))
		return 0;

	fstype = buffer;
	fstype += lxc_char_left_gc(fstype, strlen(fstype));
	fstype[lxc_char_right_gc(fstype, strlen(fstype))] = '\0';

	 
	if (fstype[0] == '\0' || fstype[0] == '#')
		return 0;

	DEBUG("trying to mount '%s'->'%s' with fstype '%s'",
	      cbarg->rootfs, cbarg->target, fstype);

	if (parse_mntopts(cbarg->options, &mntflags, &mntdata) < 0) {
		free(mntdata);
		return -1;
	}

	if (mount(cbarg->rootfs, cbarg->target, fstype, mntflags, mntdata)) {
		DEBUG("mount failed with error: %s", strerror(errno));
		free(mntdata);
		return 0;
	}
	free(mntdata);

	INFO("mounted '%s' on '%s', with fstype '%s'",
	     cbarg->rootfs, cbarg->target, fstype);

	return 1;
}