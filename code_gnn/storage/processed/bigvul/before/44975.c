xfs_attr_rmtval_remove(
	struct xfs_da_args	*args)
{
	struct xfs_mount	*mp = args->dp->i_mount;
	xfs_dablk_t		lblkno;
	int			blkcnt;
	int			error;
	int			done;

	trace_xfs_attr_rmtval_remove(args);

	 
	lblkno = args->rmtblkno;
	blkcnt = args->rmtblkcnt;
	while (blkcnt > 0) {
		struct xfs_bmbt_irec	map;
		struct xfs_buf		*bp;
		xfs_daddr_t		dblkno;
		int			dblkcnt;
		int			nmap;

		 
		nmap = 1;
		error = xfs_bmapi_read(args->dp, (xfs_fileoff_t)lblkno,
				       blkcnt, &map, &nmap, XFS_BMAPI_ATTRFORK);
		if (error)
			return(error);
		ASSERT(nmap == 1);
		ASSERT((map.br_startblock != DELAYSTARTBLOCK) &&
		       (map.br_startblock != HOLESTARTBLOCK));

		dblkno = XFS_FSB_TO_DADDR(mp, map.br_startblock),
		dblkcnt = XFS_FSB_TO_BB(mp, map.br_blockcount);

		 
		bp = xfs_incore(mp->m_ddev_targp, dblkno, dblkcnt, XBF_TRYLOCK);
		if (bp) {
			xfs_buf_stale(bp);
			xfs_buf_relse(bp);
			bp = NULL;
		}

		lblkno += map.br_blockcount;
		blkcnt -= map.br_blockcount;
	}

	 
	lblkno = args->rmtblkno;
	blkcnt = args->rmtblkcnt;
	done = 0;
	while (!done) {
		int committed;

		xfs_bmap_init(args->flist, args->firstblock);
		error = xfs_bunmapi(args->trans, args->dp, lblkno, blkcnt,
				    XFS_BMAPI_ATTRFORK | XFS_BMAPI_METADATA,
				    1, args->firstblock, args->flist,
				    &done);
		if (!error) {
			error = xfs_bmap_finish(&args->trans, args->flist,
						&committed);
		}
		if (error) {
			ASSERT(committed);
			args->trans = NULL;
			xfs_bmap_cancel(args->flist);
			return error;
		}

		 
		if (committed)
			xfs_trans_ijoin(args->trans, args->dp, 0);

		 
		error = xfs_trans_roll(&args->trans, args->dp);
		if (error)
			return (error);
	}
	return(0);
}
