xfs_bounce_unaligned_dio_write(
	struct xfs_inode	*ip,
	xfs_fileoff_t		offset_fsb,
	struct xfs_bmbt_irec	*imap)
{
	struct xfs_bmbt_irec	irec;
	xfs_fileoff_t		delta;
	bool			shared;
	bool			x;
	int			error;

	irec = *imap;
	if (offset_fsb > irec.br_startoff) {
		delta = offset_fsb - irec.br_startoff;
		irec.br_blockcount -= delta;
		irec.br_startblock += delta;
		irec.br_startoff = offset_fsb;
	}
	error = xfs_reflink_trim_around_shared(ip, &irec, &shared, &x);
	if (error)
		return error;

	 
	if (shared) {
		trace_xfs_reflink_bounce_dio_write(ip, imap);
		return -EREMCHG;
	}
	return 0;
}
