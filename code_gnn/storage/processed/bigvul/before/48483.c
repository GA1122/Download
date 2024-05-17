static int proc_sys_permission(struct inode *inode, int mask)
{
	 
	struct ctl_table_header *head;
	struct ctl_table *table;
	int error;

	 
	if ((mask & MAY_EXEC) && S_ISREG(inode->i_mode))
		return -EACCES;

	head = grab_header(inode);
	if (IS_ERR(head))
		return PTR_ERR(head);

	table = PROC_I(inode)->sysctl_entry;
	if (!table)  
		error = mask & MAY_WRITE ? -EACCES : 0;
	else  
		error = sysctl_perm(head, table, mask & ~MAY_NOT_BLOCK);

	sysctl_head_finish(head);
	return error;
}
