xfs_buf_iorequest(
	xfs_buf_t		*bp)
{
	trace_xfs_buf_iorequest(bp, _RET_IP_);

	ASSERT(!(bp->b_flags & _XBF_DELWRI_Q));

	if (bp->b_flags & XBF_WRITE)
		xfs_buf_wait_unpin(bp);
	xfs_buf_hold(bp);

	 
	atomic_set(&bp->b_io_remaining, 1);
	_xfs_buf_ioapply(bp);
	_xfs_buf_ioend(bp, 1);

	xfs_buf_rele(bp);
}
