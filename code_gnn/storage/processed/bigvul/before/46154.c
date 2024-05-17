static inline int __get_file_write_access(struct inode *inode,
					  struct vfsmount *mnt)
{
	int error;
	error = get_write_access(inode);
	if (error)
		return error;
	 
	if (!special_file(inode->i_mode)) {
		 
		error = __mnt_want_write(mnt);
		if (error)
			put_write_access(inode);
	}
	return error;
}
