static int nfs_vmtruncate(struct inode * inode, loff_t offset)
{
	if (i_size_read(inode) < offset) {
		unsigned long limit;

		limit = current->signal->rlim[RLIMIT_FSIZE].rlim_cur;
		if (limit != RLIM_INFINITY && offset > limit)
			goto out_sig;
		if (offset > inode->i_sb->s_maxbytes)
			goto out_big;
		spin_lock(&inode->i_lock);
		i_size_write(inode, offset);
		spin_unlock(&inode->i_lock);
	} else {
		struct address_space *mapping = inode->i_mapping;

		 
		if (IS_SWAPFILE(inode))
			return -ETXTBSY;
		spin_lock(&inode->i_lock);
		i_size_write(inode, offset);
		spin_unlock(&inode->i_lock);

		 
		unmap_mapping_range(mapping, offset + PAGE_SIZE - 1, 0, 1);
		truncate_inode_pages(mapping, offset);
		unmap_mapping_range(mapping, offset + PAGE_SIZE - 1, 0, 1);
	}
	return 0;
out_sig:
	send_sig(SIGXFSZ, current, 0);
out_big:
	return -EFBIG;
}
