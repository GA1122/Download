static int vfat_rmdir(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	struct super_block *sb = dir->i_sb;
	struct fat_slot_info sinfo;
	int err;

	lock_super(sb);

	err = fat_dir_empty(inode);
	if (err)
		goto out;
	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err)
		goto out;

	err = fat_remove_entries(dir, &sinfo);	 
	if (err)
		goto out;
	drop_nlink(dir);

	clear_nlink(inode);
	inode->i_mtime = inode->i_atime = CURRENT_TIME_SEC;
	fat_detach(inode);
out:
	unlock_super(sb);

	return err;
}