void lockdep_annotate_inode_mutex_key(struct inode *inode)
{
	if (S_ISDIR(inode->i_mode)) {
		struct file_system_type *type = inode->i_sb->s_type;

		 
		if (lockdep_match_class(&inode->i_mutex, &type->i_mutex_key)) {
			 
			mutex_destroy(&inode->i_mutex);
			mutex_init(&inode->i_mutex);
			lockdep_set_class(&inode->i_mutex,
					  &type->i_mutex_dir_key);
		}
	}
}