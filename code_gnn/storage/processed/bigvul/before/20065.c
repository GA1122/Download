static int hfsplus_link(struct dentry *src_dentry, struct inode *dst_dir,
			struct dentry *dst_dentry)
{
	struct hfsplus_sb_info *sbi = HFSPLUS_SB(dst_dir->i_sb);
	struct inode *inode = src_dentry->d_inode;
	struct inode *src_dir = src_dentry->d_parent->d_inode;
	struct qstr str;
	char name[32];
	u32 cnid, id;
	int res;

	if (HFSPLUS_IS_RSRC(inode))
		return -EPERM;
	if (!S_ISREG(inode->i_mode))
		return -EPERM;

	mutex_lock(&sbi->vh_mutex);
	if (inode->i_ino == (u32)(unsigned long)src_dentry->d_fsdata) {
		for (;;) {
			get_random_bytes(&id, sizeof(cnid));
			id &= 0x3fffffff;
			str.name = name;
			str.len = sprintf(name, "iNode%d", id);
			res = hfsplus_rename_cat(inode->i_ino,
						 src_dir, &src_dentry->d_name,
						 sbi->hidden_dir, &str);
			if (!res)
				break;
			if (res != -EEXIST)
				goto out;
		}
		HFSPLUS_I(inode)->linkid = id;
		cnid = sbi->next_cnid++;
		src_dentry->d_fsdata = (void *)(unsigned long)cnid;
		res = hfsplus_create_cat(cnid, src_dir,
			&src_dentry->d_name, inode);
		if (res)
			 
			goto out;
		sbi->file_count++;
	}
	cnid = sbi->next_cnid++;
	res = hfsplus_create_cat(cnid, dst_dir, &dst_dentry->d_name, inode);
	if (res)
		goto out;

	inc_nlink(inode);
	hfsplus_instantiate(dst_dentry, inode, cnid);
	ihold(inode);
	inode->i_ctime = CURRENT_TIME_SEC;
	mark_inode_dirty(inode);
	sbi->file_count++;
	dst_dir->i_sb->s_dirt = 1;
out:
	mutex_unlock(&sbi->vh_mutex);
	return res;
}