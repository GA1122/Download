int ext4_orphan_add(handle_t *handle, struct inode *inode)
{
	struct super_block *sb = inode->i_sb;
	struct ext4_iloc iloc;
	int err = 0, rc;

	if (!EXT4_SB(sb)->s_journal)
		return 0;

	mutex_lock(&EXT4_SB(sb)->s_orphan_lock);
	if (!list_empty(&EXT4_I(inode)->i_orphan))
		goto out_unlock;

	 
	J_ASSERT((S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode) ||
		  S_ISLNK(inode->i_mode)) || inode->i_nlink == 0);

	BUFFER_TRACE(EXT4_SB(sb)->s_sbh, "get_write_access");
	err = ext4_journal_get_write_access(handle, EXT4_SB(sb)->s_sbh);
	if (err)
		goto out_unlock;

	err = ext4_reserve_inode_write(handle, inode, &iloc);
	if (err)
		goto out_unlock;
	 
	if (NEXT_ORPHAN(inode) && NEXT_ORPHAN(inode) <=
		(le32_to_cpu(EXT4_SB(sb)->s_es->s_inodes_count)))
			goto mem_insert;

	 
	NEXT_ORPHAN(inode) = le32_to_cpu(EXT4_SB(sb)->s_es->s_last_orphan);
	EXT4_SB(sb)->s_es->s_last_orphan = cpu_to_le32(inode->i_ino);
	err = ext4_handle_dirty_super(handle, sb);
	rc = ext4_mark_iloc_dirty(handle, inode, &iloc);
	if (!err)
		err = rc;

	 
mem_insert:
	if (!err)
		list_add(&EXT4_I(inode)->i_orphan, &EXT4_SB(sb)->s_orphan);

	jbd_debug(4, "superblock will point to %lu\n", inode->i_ino);
	jbd_debug(4, "orphan inode %lu will point to %d\n",
			inode->i_ino, NEXT_ORPHAN(inode));
out_unlock:
	mutex_unlock(&EXT4_SB(sb)->s_orphan_lock);
	ext4_std_error(inode->i_sb, err);
	return err;
}