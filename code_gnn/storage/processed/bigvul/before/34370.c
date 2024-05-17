static ssize_t check_direct_IO(struct btrfs_root *root, int rw, struct kiocb *iocb,
			const struct iovec *iov, loff_t offset,
			unsigned long nr_segs)
{
	int seg;
	int i;
	size_t size;
	unsigned long addr;
	unsigned blocksize_mask = root->sectorsize - 1;
	ssize_t retval = -EINVAL;
	loff_t end = offset;

	if (offset & blocksize_mask)
		goto out;

	 
	for (seg = 0; seg < nr_segs; seg++) {
		addr = (unsigned long)iov[seg].iov_base;
		size = iov[seg].iov_len;
		end += size;
		if ((addr & blocksize_mask) || (size & blocksize_mask))
			goto out;

		 
		if (rw & WRITE)
			continue;

		 
		for (i = seg + 1; i < nr_segs; i++) {
			if (iov[seg].iov_base == iov[i].iov_base)
				goto out;
		}
	}
	retval = 0;
out:
	return retval;
}