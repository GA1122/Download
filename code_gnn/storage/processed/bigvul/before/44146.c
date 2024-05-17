void pipe_lock(struct pipe_inode_info *pipe)
{
	 
	pipe_lock_nested(pipe, I_MUTEX_PARENT);
}
