static int mem_open(struct inode* inode, struct file* file)
{
	file->private_data = (void*)((long)current->self_exec_id);
	 
	file->f_mode |= FMODE_UNSIGNED_OFFSET;
	return 0;
}
