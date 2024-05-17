static void pretty_print_dev(blkid_dev dev)
{
	blkid_tag_iterate	iter;
	const char		*type, *value, *devname;
	const char		*uuid = "", *fs_type = "", *label = "";
	int			len, mount_flags;
	char			mtpt[80];
	int			retval;

	if (dev == NULL) {
		pretty_print_line("device", "fs_type", "label",
				  "mount point", "UUID");
		for (len=get_terminal_width()-1; len > 0; len--)
			fputc('-', stdout);
		fputc('\n', stdout);
		return;
	}

	devname = blkid_dev_devname(dev);
	if (access(devname, F_OK))
		return;

	 
	iter = blkid_tag_iterate_begin(dev);
	while (blkid_tag_next(iter, &type, &value) == 0) {
		if (!strcmp(type, "UUID"))
			uuid = value;
		if (!strcmp(type, "TYPE"))
			fs_type = value;
		if (!strcmp(type, "LABEL"))
			label = value;
	}
	blkid_tag_iterate_end(iter);

	 
	mtpt[0] = 0;
	retval = check_mount_point(devname, &mount_flags, mtpt, sizeof(mtpt));
	if (retval == 0) {
		if (mount_flags & MF_MOUNTED) {
			if (!mtpt[0])
				strcpy(mtpt, "(mounted, mtpt unknown)");
		} else if (mount_flags & MF_BUSY)
			strcpy(mtpt, "(in use)");
		else
			strcpy(mtpt, "(not mounted)");
	}

	pretty_print_line(devname, fs_type, label, mtpt, uuid);
}