int notify_change(struct dentry * dentry, struct iattr * attr, struct inode **delegated_inode)
{
	struct inode *inode = dentry->d_inode;
	umode_t mode = inode->i_mode;
	int error;
	struct timespec now;
	unsigned int ia_valid = attr->ia_valid;

	WARN_ON_ONCE(!mutex_is_locked(&inode->i_mutex));

	if (ia_valid & (ATTR_MODE | ATTR_UID | ATTR_GID | ATTR_TIMES_SET)) {
		if (IS_IMMUTABLE(inode) || IS_APPEND(inode))
			return -EPERM;
	}

	if ((ia_valid & ATTR_MODE)) {
		umode_t amode = attr->ia_mode;
		 
		if (is_sxid(amode))
			inode->i_flags &= ~S_NOSEC;
	}

	now = current_fs_time(inode->i_sb);

	attr->ia_ctime = now;
	if (!(ia_valid & ATTR_ATIME_SET))
		attr->ia_atime = now;
	if (!(ia_valid & ATTR_MTIME_SET))
		attr->ia_mtime = now;
	if (ia_valid & ATTR_KILL_PRIV) {
		attr->ia_valid &= ~ATTR_KILL_PRIV;
		ia_valid &= ~ATTR_KILL_PRIV;
		error = security_inode_need_killpriv(dentry);
		if (error > 0)
			error = security_inode_killpriv(dentry);
		if (error)
			return error;
	}

	 
	if ((ia_valid & (ATTR_KILL_SUID|ATTR_KILL_SGID)) &&
	    (ia_valid & ATTR_MODE))
		BUG();

	if (ia_valid & ATTR_KILL_SUID) {
		if (mode & S_ISUID) {
			ia_valid = attr->ia_valid |= ATTR_MODE;
			attr->ia_mode = (inode->i_mode & ~S_ISUID);
		}
	}
	if (ia_valid & ATTR_KILL_SGID) {
		if ((mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP)) {
			if (!(ia_valid & ATTR_MODE)) {
				ia_valid = attr->ia_valid |= ATTR_MODE;
				attr->ia_mode = inode->i_mode;
			}
			attr->ia_mode &= ~S_ISGID;
		}
	}
	if (!(attr->ia_valid & ~(ATTR_KILL_SUID | ATTR_KILL_SGID)))
		return 0;

	error = security_inode_setattr(dentry, attr);
	if (error)
		return error;
	error = try_break_deleg(inode, delegated_inode);
	if (error)
		return error;

	if (inode->i_op->setattr)
		error = inode->i_op->setattr(dentry, attr);
	else
		error = simple_setattr(dentry, attr);

	if (!error) {
		fsnotify_change(dentry, ia_valid);
		ima_inode_post_setattr(dentry);
		evm_inode_post_setattr(dentry, ia_valid);
	}

	return error;
}