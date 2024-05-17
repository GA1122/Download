static int apparmor_file_open(struct file *file, const struct cred *cred)
{
	struct aa_file_cxt *fcxt = file->f_security;
	struct aa_profile *profile;
	int error = 0;

	if (!mediated_filesystem(file->f_path.dentry))
		return 0;

	 
	if (current->in_execve) {
		fcxt->allow = MAY_EXEC | MAY_READ | AA_EXEC_MMAP;
		return 0;
	}

	profile = aa_cred_profile(cred);
	if (!unconfined(profile)) {
		struct inode *inode = file_inode(file);
		struct path_cond cond = { inode->i_uid, inode->i_mode };

		error = aa_path_perm(OP_OPEN, profile, &file->f_path, 0,
				     aa_map_file_to_perms(file), &cond);
		 
		fcxt->allow = aa_map_file_to_perms(file);
	}

	return error;
}