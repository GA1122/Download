static int add_dirent_to_buf(handle_t *handle, struct dentry *dentry,
			     struct inode *inode, struct ext4_dir_entry_2 *de,
			     struct buffer_head *bh)
{
	struct inode	*dir = dentry->d_parent->d_inode;
	const char	*name = dentry->d_name.name;
	int		namelen = dentry->d_name.len;
	unsigned int	offset = 0;
	unsigned int	blocksize = dir->i_sb->s_blocksize;
	unsigned short	reclen;
	int		nlen, rlen, err;
	char		*top;
	int		csum_size = 0;

	if (EXT4_HAS_RO_COMPAT_FEATURE(inode->i_sb,
				       EXT4_FEATURE_RO_COMPAT_METADATA_CSUM))
		csum_size = sizeof(struct ext4_dir_entry_tail);

	reclen = EXT4_DIR_REC_LEN(namelen);
	if (!de) {
		de = (struct ext4_dir_entry_2 *)bh->b_data;
		top = bh->b_data + (blocksize - csum_size) - reclen;
		while ((char *) de <= top) {
			if (ext4_check_dir_entry(dir, NULL, de, bh, offset))
				return -EIO;
			if (ext4_match(namelen, name, de))
				return -EEXIST;
			nlen = EXT4_DIR_REC_LEN(de->name_len);
			rlen = ext4_rec_len_from_disk(de->rec_len, blocksize);
			if ((de->inode? rlen - nlen: rlen) >= reclen)
				break;
			de = (struct ext4_dir_entry_2 *)((char *)de + rlen);
			offset += rlen;
		}
		if ((char *) de > top)
			return -ENOSPC;
	}
	BUFFER_TRACE(bh, "get_write_access");
	err = ext4_journal_get_write_access(handle, bh);
	if (err) {
		ext4_std_error(dir->i_sb, err);
		return err;
	}

	 
	nlen = EXT4_DIR_REC_LEN(de->name_len);
	rlen = ext4_rec_len_from_disk(de->rec_len, blocksize);
	if (de->inode) {
		struct ext4_dir_entry_2 *de1 = (struct ext4_dir_entry_2 *)((char *)de + nlen);
		de1->rec_len = ext4_rec_len_to_disk(rlen - nlen, blocksize);
		de->rec_len = ext4_rec_len_to_disk(nlen, blocksize);
		de = de1;
	}
	de->file_type = EXT4_FT_UNKNOWN;
	de->inode = cpu_to_le32(inode->i_ino);
	ext4_set_de_type(dir->i_sb, de, inode->i_mode);
	de->name_len = namelen;
	memcpy(de->name, name, namelen);
	 
	dir->i_mtime = dir->i_ctime = ext4_current_time(dir);
	ext4_update_dx_flag(dir);
	dir->i_version++;
	ext4_mark_inode_dirty(handle, dir);
	BUFFER_TRACE(bh, "call ext4_handle_dirty_metadata");
	err = ext4_handle_dirty_dirent_node(handle, dir, bh);
	if (err)
		ext4_std_error(dir->i_sb, err);
	return 0;
}