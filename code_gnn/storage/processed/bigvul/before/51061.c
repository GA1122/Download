static int ovl_do_remove(struct dentry *dentry, bool is_dir)
{
	enum ovl_path_type type;
	int err;

	err = ovl_check_sticky(dentry);
	if (err)
		goto out;

	err = ovl_want_write(dentry);
	if (err)
		goto out;

	err = ovl_copy_up(dentry->d_parent);
	if (err)
		goto out_drop_write;

	type = ovl_path_type(dentry);
	if (OVL_TYPE_PURE_UPPER(type)) {
		err = ovl_remove_upper(dentry, is_dir);
	} else {
		const struct cred *old_cred;
		struct cred *override_cred;

		err = -ENOMEM;
		override_cred = prepare_creds();
		if (!override_cred)
			goto out_drop_write;

		 
		cap_raise(override_cred->cap_effective, CAP_SYS_ADMIN);
		cap_raise(override_cred->cap_effective, CAP_DAC_OVERRIDE);
		cap_raise(override_cred->cap_effective, CAP_FOWNER);
		cap_raise(override_cred->cap_effective, CAP_FSETID);
		cap_raise(override_cred->cap_effective, CAP_CHOWN);
		old_cred = override_creds(override_cred);

		err = ovl_remove_and_whiteout(dentry, is_dir);

		revert_creds(old_cred);
		put_cred(override_cred);
	}
out_drop_write:
	ovl_drop_write(dentry);
out:
	return err;
}