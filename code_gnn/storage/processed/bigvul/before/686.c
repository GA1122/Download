int open_tmpfile(char *fnametmp, const char *fname, struct file_struct *file)
{
	int fd;
	mode_t added_perms;

	if (!get_tmpname(fnametmp, fname, False))
		return -1;

	if (am_root < 0) {
		 
		added_perms = S_IRUSR|S_IWUSR;
	} else {
		 
		added_perms = S_IWUSR;
	}

	 
	fd = do_mkstemp(fnametmp, (file->mode|added_perms) & INITACCESSPERMS);

#if 0
	 
	if (fd == -1 && relative_paths && errno == ENOENT
	 && make_path(fnametmp, MKP_SKIP_SLASH | MKP_DROP_NAME) == 0) {
		 
		get_tmpname(fnametmp, fname, False);
		fd = do_mkstemp(fnametmp, (file->mode|added_perms) & INITACCESSPERMS);
	}
#endif

	if (fd == -1) {
		rsyserr(FERROR_XFER, errno, "mkstemp %s failed",
			full_fname(fnametmp));
		return -1;
	}

	return fd;
}
