xfs_buf_bio_end_io(
	struct bio		*bio,
	int			error)
{
	xfs_buf_t		*bp = (xfs_buf_t *)bio->bi_private;

	 
	if (!bp->b_error)
		xfs_buf_ioerror(bp, -error);

	if (!bp->b_error && xfs_buf_is_vmapped(bp) && (bp->b_flags & XBF_READ))
		invalidate_kernel_vmap_range(bp->b_addr, xfs_buf_vmap_len(bp));

	_xfs_buf_ioend(bp, 1);
	bio_put(bio);
}
