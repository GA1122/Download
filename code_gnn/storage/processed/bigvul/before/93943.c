xfs_destroy_ioend(
	struct xfs_ioend	*ioend,
	int			error)
{
	struct inode		*inode = ioend->io_inode;
	struct bio		*last = ioend->io_bio;
	struct bio		*bio, *next;

	for (bio = &ioend->io_inline_bio; bio; bio = next) {
		struct bio_vec	*bvec;
		int		i;

		 
		if (bio == last)
			next = NULL;
		else
			next = bio->bi_private;

		 
		bio_for_each_segment_all(bvec, bio, i)
			xfs_finish_page_writeback(inode, bvec, error);

		bio_put(bio);
	}
}
