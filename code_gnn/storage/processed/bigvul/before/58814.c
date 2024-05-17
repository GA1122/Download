generic_file_direct_IO(int rw, struct kiocb *iocb, const struct iovec *iov,
	loff_t offset, unsigned long nr_segs)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	ssize_t retval;
	size_t write_len;
	pgoff_t end = 0;  

	 
	if (rw == WRITE) {
		write_len = iov_length(iov, nr_segs);
		end = (offset + write_len - 1) >> PAGE_CACHE_SHIFT;
	       	if (mapping_mapped(mapping))
			unmap_mapping_range(mapping, offset, write_len, 0);
	}

	retval = filemap_write_and_wait(mapping);
	if (retval)
		goto out;

	 
	if (rw == WRITE && mapping->nrpages) {
		retval = invalidate_inode_pages2_range(mapping,
					offset >> PAGE_CACHE_SHIFT, end);
		if (retval)
			goto out;
	}

	retval = mapping->a_ops->direct_IO(rw, iocb, iov, offset, nr_segs);

	 
	if (rw == WRITE && mapping->nrpages) {
		invalidate_inode_pages2_range(mapping, offset >> PAGE_CACHE_SHIFT, end);
	}
out:
	return retval;
}
