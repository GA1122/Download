check_fstab(const char *progname, char *mountpoint, char *devname,
	    char **options)
{
	FILE *fstab;
	struct mntent *mnt;

	 
	fstab = setmntent(_PATH_FSTAB, "r");
	if (!fstab) {
		fprintf(stderr, "Couldn't open %s for reading!\n",
				_PATH_FSTAB);
		return EX_FILEIO;
	}

	while((mnt = getmntent(fstab))) {
		if (!strcmp(mountpoint, mnt->mnt_dir))
			break;
	}
	endmntent(fstab);

	if (mnt == NULL || strcmp(mnt->mnt_fsname, devname)) {
		fprintf(stderr, "%s: permission denied: no match for "
				"%s found in %s\n", progname, mountpoint,
				_PATH_FSTAB);
		return EX_USAGE;
	}

	 
	free(*options);
	*options = strdup(mnt->mnt_opts);
	return 0;
}
