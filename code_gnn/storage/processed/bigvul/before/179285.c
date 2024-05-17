 _xfs_buf_find(
 	struct xfs_buftarg	*btp,
 	struct xfs_buf_map	*map,
 	int			nmaps,
 	xfs_buf_flags_t		flags,
 	xfs_buf_t		*new_bp)
 {
 	size_t			numbytes;
 	struct xfs_perag	*pag;
 	struct rb_node		**rbp;
  	struct rb_node		*parent;
  	xfs_buf_t		*bp;
  	xfs_daddr_t		blkno = map[0].bm_bn;
// 	xfs_daddr_t		eofs;
  	int			numblks = 0;
  	int			i;
  
 	for (i = 0; i < nmaps; i++)
 		numblks += map[i].bm_len;
 	numbytes = BBTOB(numblks);
 
 	 
  	ASSERT(!(numbytes < (1 << btp->bt_sshift)));
  	ASSERT(!(BBTOB(blkno) & (xfs_off_t)btp->bt_smask));
  
// 	 
// 	eofs = XFS_FSB_TO_BB(btp->bt_mount, btp->bt_mount->m_sb.sb_dblocks);
// 	if (blkno >= eofs) {
// 		 
// 		xfs_alert(btp->bt_mount,
// 			  "%s: Block out of range: block 0x%llx, EOFS 0x%llx ",
// 			  __func__, blkno, eofs);
// 		return NULL;
// 	}
// 
  	 
  	pag = xfs_perag_get(btp->bt_mount,
  				xfs_daddr_to_agno(btp->bt_mount, blkno));
 
 	 
 	spin_lock(&pag->pag_buf_lock);
 	rbp = &pag->pag_buf_tree.rb_node;
 	parent = NULL;
 	bp = NULL;
 	while (*rbp) {
 		parent = *rbp;
 		bp = rb_entry(parent, struct xfs_buf, b_rbnode);
 
 		if (blkno < bp->b_bn)
 			rbp = &(*rbp)->rb_left;
 		else if (blkno > bp->b_bn)
 			rbp = &(*rbp)->rb_right;
 		else {
 			 
 			if (bp->b_length != numblks) {
 				ASSERT(bp->b_flags & XBF_STALE);
 				rbp = &(*rbp)->rb_right;
 				continue;
 			}
 			atomic_inc(&bp->b_hold);
 			goto found;
 		}
 	}
 
 	 
 	if (new_bp) {
 		rb_link_node(&new_bp->b_rbnode, parent, rbp);
 		rb_insert_color(&new_bp->b_rbnode, &pag->pag_buf_tree);
 		 
 		new_bp->b_pag = pag;
 		spin_unlock(&pag->pag_buf_lock);
 	} else {
 		XFS_STATS_INC(xb_miss_locked);
 		spin_unlock(&pag->pag_buf_lock);
 		xfs_perag_put(pag);
 	}
 	return new_bp;
 
 found:
 	spin_unlock(&pag->pag_buf_lock);
 	xfs_perag_put(pag);
 
 	if (!xfs_buf_trylock(bp)) {
 		if (flags & XBF_TRYLOCK) {
 			xfs_buf_rele(bp);
 			XFS_STATS_INC(xb_busy_locked);
 			return NULL;
 		}
 		xfs_buf_lock(bp);
 		XFS_STATS_INC(xb_get_locked_waited);
 	}
 
 	 
 	if (bp->b_flags & XBF_STALE) {
 		ASSERT((bp->b_flags & _XBF_DELWRI_Q) == 0);
 		ASSERT(bp->b_iodone == NULL);
 		bp->b_flags &= _XBF_KMEM | _XBF_PAGES;
 		bp->b_ops = NULL;
 	}
 
 	trace_xfs_buf_find(bp, flags, _RET_IP_);
 	XFS_STATS_INC(xb_get_locked);
 	return bp;
 }