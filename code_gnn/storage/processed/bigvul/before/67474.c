void fsnotify_unmount_inodes(struct super_block *sb)
{
	struct inode *inode, *iput_inode = NULL;

	spin_lock(&sb->s_inode_list_lock);
	list_for_each_entry(inode, &sb->s_inodes, i_sb_list) {
		 
		spin_lock(&inode->i_lock);
		if (inode->i_state & (I_FREEING|I_WILL_FREE|I_NEW)) {
			spin_unlock(&inode->i_lock);
			continue;
		}

		 
		if (!atomic_read(&inode->i_count)) {
			spin_unlock(&inode->i_lock);
			continue;
		}

		__iget(inode);
		spin_unlock(&inode->i_lock);
		spin_unlock(&sb->s_inode_list_lock);

		if (iput_inode)
			iput(iput_inode);

		 
		fsnotify(inode, FS_UNMOUNT, inode, FSNOTIFY_EVENT_INODE, NULL, 0);

		fsnotify_inode_delete(inode);

		iput_inode = inode;

		spin_lock(&sb->s_inode_list_lock);
	}
	spin_unlock(&sb->s_inode_list_lock);

	if (iput_inode)
		iput(iput_inode);
}