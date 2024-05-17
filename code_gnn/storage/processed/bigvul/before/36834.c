void clear_inode(struct inode *inode)
{
	might_sleep();
	 
	spin_lock_irq(&inode->i_data.tree_lock);
	BUG_ON(inode->i_data.nrpages);
	BUG_ON(inode->i_data.nrshadows);
	spin_unlock_irq(&inode->i_data.tree_lock);
	BUG_ON(!list_empty(&inode->i_data.private_list));
	BUG_ON(!(inode->i_state & I_FREEING));
	BUG_ON(inode->i_state & I_CLEAR);
	 
	inode->i_state = I_FREEING | I_CLEAR;
}
