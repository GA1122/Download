struct file *dentry_open(const struct path *path, int flags,
			 const struct cred *cred)
{
	int error;
	struct file *f;

	validate_creds(cred);

	 
	BUG_ON(!path->mnt);

	f = get_empty_filp();
	if (!IS_ERR(f)) {
		f->f_flags = flags;
		f->f_path = *path;
		error = do_dentry_open(f, NULL, cred);
		if (!error) {
			 
			error = open_check_o_direct(f);
			if (error) {
				fput(f);
				f = ERR_PTR(error);
			}
		} else { 
			put_filp(f);
			f = ERR_PTR(error);
		}
	}
	return f;
}
