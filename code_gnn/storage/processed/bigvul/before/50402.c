reiserfs_set_acl(struct inode *inode, struct posix_acl *acl, int type)
{
	int error, error2;
	struct reiserfs_transaction_handle th;
	size_t jcreate_blocks;
	int size = acl ? posix_acl_xattr_size(acl->a_count) : 0;


	 

	jcreate_blocks = reiserfs_xattr_jcreate_nblocks(inode) +
			 reiserfs_xattr_nblocks(inode, size) * 2;

	reiserfs_write_lock(inode->i_sb);
	error = journal_begin(&th, inode->i_sb, jcreate_blocks);
	reiserfs_write_unlock(inode->i_sb);
	if (error == 0) {
		error = __reiserfs_set_acl(&th, inode, type, acl);
		reiserfs_write_lock(inode->i_sb);
		error2 = journal_end(&th);
		reiserfs_write_unlock(inode->i_sb);
		if (error2)
			error = error2;
	}

	return error;
}