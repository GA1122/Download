static struct dentry *vfat_lookup(struct inode *dir, struct dentry *dentry,
				  struct nameidata *nd)
{
	struct super_block *sb = dir->i_sb;
	struct fat_slot_info sinfo;
	struct inode *inode;
	struct dentry *alias;
	int err;

	lock_super(sb);

	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err) {
		if (err == -ENOENT) {
			inode = NULL;
			goto out;
		}
		goto error;
	}

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
	brelse(sinfo.bh);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto error;
	}

	alias = d_find_alias(inode);
	if (alias && !vfat_d_anon_disconn(alias)) {
		 
		BUG_ON(d_unhashed(alias));
		if (!S_ISDIR(inode->i_mode))
			d_move(alias, dentry);
		iput(inode);
		unlock_super(sb);
		return alias;
	} else
		dput(alias);

out:
	unlock_super(sb);
	dentry->d_time = dentry->d_parent->d_inode->i_version;
	dentry = d_splice_alias(inode, dentry);
	if (dentry)
		dentry->d_time = dentry->d_parent->d_inode->i_version;
	return dentry;

error:
	unlock_super(sb);
	return ERR_PTR(err);
}