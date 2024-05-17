static int ovl_create_or_link(struct dentry *dentry, int mode, dev_t rdev,
			      const char *link, struct dentry *hardlink)
{
	int err;
	struct inode *inode;
	struct kstat stat = {
		.mode = mode,
		.rdev = rdev,
	};

	err = -ENOMEM;
	inode = ovl_new_inode(dentry->d_sb, mode, dentry->d_fsdata);
	if (!inode)
		goto out;

	err = ovl_copy_up(dentry->d_parent);
	if (err)
		goto out_iput;

	if (!ovl_dentry_is_opaque(dentry)) {
		err = ovl_create_upper(dentry, inode, &stat, link, hardlink);
	} else {
		const struct cred *old_cred;
		struct cred *override_cred;

		err = -ENOMEM;
		override_cred = prepare_creds();
		if (!override_cred)
			goto out_iput;

		 
		cap_raise(override_cred->cap_effective, CAP_SYS_ADMIN);
		cap_raise(override_cred->cap_effective, CAP_DAC_OVERRIDE);
		cap_raise(override_cred->cap_effective, CAP_FOWNER);
		old_cred = override_creds(override_cred);

		err = ovl_create_over_whiteout(dentry, inode, &stat, link,
					       hardlink);

		revert_creds(old_cred);
		put_cred(override_cred);
	}

	if (!err)
		inode = NULL;
out_iput:
	iput(inode);
out:
	return err;
}