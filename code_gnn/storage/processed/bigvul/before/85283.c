struct page *new_inode_page(struct inode *inode)
{
	struct dnode_of_data dn;

	 
	set_new_dnode(&dn, inode, NULL, NULL, inode->i_ino);

	 
	return new_node_page(&dn, 0, NULL);
}
