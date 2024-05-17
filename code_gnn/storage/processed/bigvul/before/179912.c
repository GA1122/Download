 xfs_attr_rmtval_set(
 	struct xfs_da_args	*args)
 {
 	struct xfs_inode	*dp = args->dp;
 	struct xfs_mount	*mp = dp->i_mount;
 	struct xfs_bmbt_irec	map;
 	xfs_dablk_t		lblkno;
 	xfs_fileoff_t		lfileoff = 0;
 	__uint8_t		*src = args->value;
 	int			blkcnt;
 	int			valuelen;
 	int			nmap;
 	int			error;
 	int			offset = 0;
 
 	trace_xfs_attr_rmtval_set(args);
 
 	 
	blkcnt = xfs_attr3_rmt_blocks(mp, args->valuelen);
// 	blkcnt = xfs_attr3_rmt_blocks(mp, args->rmtvaluelen);
  	error = xfs_bmap_first_unused(args->trans, args->dp, blkcnt, &lfileoff,
  						   XFS_ATTR_FORK);
  	if (error)
 		return error;
 
 	args->rmtblkno = lblkno = (xfs_dablk_t)lfileoff;
 	args->rmtblkcnt = blkcnt;
 
 	 
 	while (blkcnt > 0) {
 		int	committed;
 
 		 
 		xfs_bmap_init(args->flist, args->firstblock);
 		nmap = 1;
 		error = xfs_bmapi_write(args->trans, dp, (xfs_fileoff_t)lblkno,
 				  blkcnt,
 				  XFS_BMAPI_ATTRFORK | XFS_BMAPI_METADATA,
 				  args->firstblock, args->total, &map, &nmap,
 				  args->flist);
 		if (!error) {
 			error = xfs_bmap_finish(&args->trans, args->flist,
 						&committed);
 		}
 		if (error) {
 			ASSERT(committed);
 			args->trans = NULL;
 			xfs_bmap_cancel(args->flist);
 			return(error);
 		}
 
 		 
 		if (committed)
 			xfs_trans_ijoin(args->trans, dp, 0);
 
 		ASSERT(nmap == 1);
 		ASSERT((map.br_startblock != DELAYSTARTBLOCK) &&
 		       (map.br_startblock != HOLESTARTBLOCK));
 		lblkno += map.br_blockcount;
 		blkcnt -= map.br_blockcount;
 
 		 
 		error = xfs_trans_roll(&args->trans, dp);
 		if (error)
 			return (error);
 	}
 
 	 
  	lblkno = args->rmtblkno;
  	blkcnt = args->rmtblkcnt;
	valuelen = args->valuelen;
// 	valuelen = args->rmtvaluelen;
  	while (valuelen > 0) {
  		struct xfs_buf	*bp;
  		xfs_daddr_t	dblkno;
 		int		dblkcnt;
 
 		ASSERT(blkcnt > 0);
 
 		xfs_bmap_init(args->flist, args->firstblock);
 		nmap = 1;
 		error = xfs_bmapi_read(dp, (xfs_fileoff_t)lblkno,
 				       blkcnt, &map, &nmap,
 				       XFS_BMAPI_ATTRFORK);
 		if (error)
 			return(error);
 		ASSERT(nmap == 1);
 		ASSERT((map.br_startblock != DELAYSTARTBLOCK) &&
 		       (map.br_startblock != HOLESTARTBLOCK));
 
 		dblkno = XFS_FSB_TO_DADDR(mp, map.br_startblock),
 		dblkcnt = XFS_FSB_TO_BB(mp, map.br_blockcount);
 
 		bp = xfs_buf_get(mp->m_ddev_targp, dblkno, dblkcnt, 0);
 		if (!bp)
 			return ENOMEM;
 		bp->b_ops = &xfs_attr3_rmt_buf_ops;
 
 		xfs_attr_rmtval_copyin(mp, bp, args->dp->i_ino, &offset,
 				       &valuelen, &src);
 
 		error = xfs_bwrite(bp);	 
 		xfs_buf_relse(bp);
 		if (error)
 			return error;
 
 
 		 
 		lblkno += map.br_blockcount;
 		blkcnt -= map.br_blockcount;
 	}
 	ASSERT(valuelen == 0);
 	return 0;
 }