static void init_once(void *foo)
{
	struct ext2_inode_info *ei = (struct ext2_inode_info *) foo;

	rwlock_init(&ei->i_meta_lock);
#ifdef CONFIG_EXT2_FS_XATTR
	init_rwsem(&ei->xattr_sem);
#endif
	mutex_init(&ei->truncate_mutex);
#ifdef CONFIG_FS_DAX
	init_rwsem(&ei->dax_sem);
#endif
	inode_init_once(&ei->vfs_inode);
}
