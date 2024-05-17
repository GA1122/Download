int reiserfs_cache_default_acl(struct inode *inode)
{
	struct posix_acl *acl;
	int nblocks = 0;

	if (IS_PRIVATE(inode))
		return 0;

	acl = get_acl(inode, ACL_TYPE_DEFAULT);

	if (acl && !IS_ERR(acl)) {
		int size = reiserfs_acl_size(acl->a_count);

		 
		nblocks = reiserfs_xattr_jcreate_nblocks(inode);
		nblocks += JOURNAL_BLOCKS_PER_OBJECT(inode->i_sb);

		REISERFS_I(inode)->i_flags |= i_has_xattr_dir;

		 
		nblocks += reiserfs_xattr_nblocks(inode, size) * 4;
		posix_acl_release(acl);
	}

	return nblocks;
}