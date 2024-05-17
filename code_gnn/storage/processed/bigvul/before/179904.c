 xfs_attr_node_addname(xfs_da_args_t *args)
 {
 	xfs_da_state_t *state;
 	xfs_da_state_blk_t *blk;
 	xfs_inode_t *dp;
 	xfs_mount_t *mp;
 	int committed, retval, error;
 
 	trace_xfs_attr_node_addname(args);
 
 	 
 	dp = args->dp;
 	mp = dp->i_mount;
 restart:
 	state = xfs_da_state_alloc();
 	state->args = args;
 	state->mp = mp;
 	state->blocksize = state->mp->m_sb.sb_blocksize;
 	state->node_ents = state->mp->m_attr_node_ents;
 
 	 
 	error = xfs_da3_node_lookup_int(state, &retval);
 	if (error)
 		goto out;
 	blk = &state->path.blk[ state->path.active-1 ];
 	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
 	if ((args->flags & ATTR_REPLACE) && (retval == ENOATTR)) {
 		goto out;
 	} else if (retval == EEXIST) {
 		if (args->flags & ATTR_CREATE)
 			goto out;
  
  		trace_xfs_attr_node_replace(args);
  
// 		 
  		args->op_flags |= XFS_DA_OP_RENAME;	 
  		args->blkno2 = args->blkno;		 
  		args->index2 = args->index;
  		args->rmtblkno2 = args->rmtblkno;
  		args->rmtblkcnt2 = args->rmtblkcnt;
// 		args->rmtvaluelen2 = args->rmtvaluelen;
// 
// 		 
  		args->rmtblkno = 0;
  		args->rmtblkcnt = 0;
// 		args->rmtvaluelen = 0;
  	}
  
  	retval = xfs_attr3_leaf_add(blk->bp, state->args);
 	if (retval == ENOSPC) {
 		if (state->path.active == 1) {
 			 
 			xfs_da_state_free(state);
 			state = NULL;
 			xfs_bmap_init(args->flist, args->firstblock);
 			error = xfs_attr3_leaf_to_node(args);
 			if (!error) {
 				error = xfs_bmap_finish(&args->trans,
 							args->flist,
 							&committed);
 			}
 			if (error) {
 				ASSERT(committed);
 				args->trans = NULL;
 				xfs_bmap_cancel(args->flist);
 				goto out;
 			}
 
 			 
 			if (committed)
 				xfs_trans_ijoin(args->trans, dp, 0);
 
 			 
 			error = xfs_trans_roll(&args->trans, dp);
 			if (error)
 				goto out;
 
 			goto restart;
 		}
 
 		 
 		xfs_bmap_init(args->flist, args->firstblock);
 		error = xfs_da3_split(state);
 		if (!error) {
 			error = xfs_bmap_finish(&args->trans, args->flist,
 						&committed);
 		}
 		if (error) {
 			ASSERT(committed);
 			args->trans = NULL;
 			xfs_bmap_cancel(args->flist);
 			goto out;
 		}
 
 		 
 		if (committed)
 			xfs_trans_ijoin(args->trans, dp, 0);
 	} else {
 		 
 		xfs_da3_fixhashpath(state, &state->path);
 	}
 
 	 
 	xfs_da_state_free(state);
 	state = NULL;
 
 	 
 	error = xfs_trans_roll(&args->trans, dp);
 	if (error)
 		goto out;
 
 	 
 	if (args->rmtblkno > 0) {
 		error = xfs_attr_rmtval_set(args);
 		if (error)
 			return(error);
 	}
 
 	 
 	if (args->op_flags & XFS_DA_OP_RENAME) {
 		 
 		error = xfs_attr3_leaf_flipflags(args);
 		if (error)
 			goto out;
 
 		 
 		args->index = args->index2;
  		args->blkno = args->blkno2;
  		args->rmtblkno = args->rmtblkno2;
  		args->rmtblkcnt = args->rmtblkcnt2;
// 		args->rmtvaluelen = args->rmtvaluelen2;
  		if (args->rmtblkno) {
  			error = xfs_attr_rmtval_remove(args);
  			if (error)
 				return(error);
 		}
 
 		 
 		args->flags |= XFS_ATTR_INCOMPLETE;
 		state = xfs_da_state_alloc();
 		state->args = args;
 		state->mp = mp;
 		state->blocksize = state->mp->m_sb.sb_blocksize;
 		state->node_ents = state->mp->m_attr_node_ents;
 		state->inleaf = 0;
 		error = xfs_da3_node_lookup_int(state, &retval);
 		if (error)
 			goto out;
 
 		 
 		blk = &state->path.blk[ state->path.active-1 ];
 		ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
 		error = xfs_attr3_leaf_remove(blk->bp, args);
 		xfs_da3_fixhashpath(state, &state->path);
 
 		 
 		if (retval && (state->path.active > 1)) {
 			xfs_bmap_init(args->flist, args->firstblock);
 			error = xfs_da3_join(state);
 			if (!error) {
 				error = xfs_bmap_finish(&args->trans,
 							args->flist,
 							&committed);
 			}
 			if (error) {
 				ASSERT(committed);
 				args->trans = NULL;
 				xfs_bmap_cancel(args->flist);
 				goto out;
 			}
 
 			 
 			if (committed)
 				xfs_trans_ijoin(args->trans, dp, 0);
 		}
 
 		 
 		error = xfs_trans_roll(&args->trans, dp);
 		if (error)
 			goto out;
 
 	} else if (args->rmtblkno > 0) {
 		 
 		error = xfs_attr3_leaf_clearflag(args);
 		if (error)
 			goto out;
 	}
 	retval = error = 0;
 
 out:
 	if (state)
 		xfs_da_state_free(state);
 	if (error)
 		return(error);
 	return(retval);
 }