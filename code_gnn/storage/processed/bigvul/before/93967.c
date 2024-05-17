xfs_submit_ioend(
	struct writeback_control *wbc,
	struct xfs_ioend	*ioend,
	int			status)
{
	 
	if (!status &&
	    ioend->io_type != XFS_IO_UNWRITTEN &&
	    xfs_ioend_is_append(ioend) &&
	    !ioend->io_append_trans)
		status = xfs_setfilesize_trans_alloc(ioend);

	ioend->io_bio->bi_private = ioend;
	ioend->io_bio->bi_end_io = xfs_end_bio;
	bio_set_op_attrs(ioend->io_bio, REQ_OP_WRITE,
			 (wbc->sync_mode == WB_SYNC_ALL) ? WRITE_SYNC : 0);
	 
	if (status) {
		ioend->io_bio->bi_error = status;
		bio_endio(ioend->io_bio);
		return status;
	}

	submit_bio(ioend->io_bio);
	return 0;
}