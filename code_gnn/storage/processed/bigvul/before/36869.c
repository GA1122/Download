void inode_init_once(struct inode *inode)
{
	memset(inode, 0, sizeof(*inode));
	INIT_HLIST_NODE(&inode->i_hash);
	INIT_LIST_HEAD(&inode->i_devices);
	INIT_LIST_HEAD(&inode->i_wb_list);
	INIT_LIST_HEAD(&inode->i_lru);
	address_space_init_once(&inode->i_data);
	i_size_ordered_init(inode);
#ifdef CONFIG_FSNOTIFY
	INIT_HLIST_HEAD(&inode->i_fsnotify_marks);
#endif
}