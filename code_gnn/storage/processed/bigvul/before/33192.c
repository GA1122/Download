_xfs_buf_ioapply(
	struct xfs_buf	*bp)
{
	struct blk_plug	plug;
	int		rw;
	int		offset;
	int		size;
	int		i;

	if (bp->b_flags & XBF_WRITE) {
		if (bp->b_flags & XBF_SYNCIO)
			rw = WRITE_SYNC;
		else
			rw = WRITE;
		if (bp->b_flags & XBF_FUA)
			rw |= REQ_FUA;
		if (bp->b_flags & XBF_FLUSH)
			rw |= REQ_FLUSH;

		 
		if (bp->b_ops) {
			bp->b_ops->verify_write(bp);
			if (bp->b_error) {
				xfs_force_shutdown(bp->b_target->bt_mount,
						   SHUTDOWN_CORRUPT_INCORE);
				return;
			}
		}
	} else if (bp->b_flags & XBF_READ_AHEAD) {
		rw = READA;
	} else {
		rw = READ;
	}

	 
	rw |= REQ_META;

	 
	offset = bp->b_offset;
	size = BBTOB(bp->b_io_length);
	blk_start_plug(&plug);
	for (i = 0; i < bp->b_map_count; i++) {
		xfs_buf_ioapply_map(bp, i, &offset, &size, rw);
		if (bp->b_error)
			break;
		if (size <= 0)
			break;	 
	}
	blk_finish_plug(&plug);
}
