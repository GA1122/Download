static ssize_t check_direct_IO(struct btrfs_root *root, struct kiocb *iocb,
			const struct iov_iter *iter, loff_t offset)
{
	int seg;
	int i;
	unsigned blocksize_mask = root->sectorsize - 1;
	ssize_t retval = -EINVAL;

	if (offset & blocksize_mask)
		goto out;

	if (iov_iter_alignment(iter) & blocksize_mask)
		goto out;

	 
	if (iov_iter_rw(iter) == WRITE)
		return 0;
	 
	for (seg = 0; seg < iter->nr_segs; seg++) {
		for (i = seg + 1; i < iter->nr_segs; i++) {
			if (iter->iov[seg].iov_base == iter->iov[i].iov_base)
				goto out;
		}
	}
	retval = 0;
out:
	return retval;
}
