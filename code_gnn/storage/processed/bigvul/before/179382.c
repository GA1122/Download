 static int gfs2_bmap_alloc(struct inode *inode, const sector_t lblock,
 			   struct buffer_head *bh_map, struct metapath *mp,
 			   const unsigned int sheight,
 			   const unsigned int height,
 			   const unsigned int maxlen)
  {
  	struct gfs2_inode *ip = GFS2_I(inode);
  	struct gfs2_sbd *sdp = GFS2_SB(inode);
// 	struct super_block *sb = sdp->sd_vfs;
  	struct buffer_head *dibh = mp->mp_bh[0];
  	u64 bn, dblock = 0;
  	unsigned n, i, blks, alloced = 0, iblks = 0, branch_start = 0;
  	unsigned dblks = 0;
  	unsigned ptrs_per_blk;
  	const unsigned end_of_metadata = height - 1;
// 	int ret;
  	int eob = 0;
  	enum alloc_state state;
  	__be64 *ptr;
 	__be64 zero_bn = 0;
 
 	BUG_ON(sheight < 1);
 	BUG_ON(dibh == NULL);
 
 	gfs2_trans_add_bh(ip->i_gl, dibh, 1);
 
 	if (height == sheight) {
 		struct buffer_head *bh;
 		 
 		ptr = metapointer(end_of_metadata, mp);
 		bh = mp->mp_bh[end_of_metadata];
 		dblks = gfs2_extent_length(bh->b_data, bh->b_size, ptr, maxlen,
 					   &eob);
 		BUG_ON(dblks < 1);
 		state = ALLOC_DATA;
 	} else {
 		 
 		ptrs_per_blk = height > 1 ? sdp->sd_inptrs : sdp->sd_diptrs;
 		dblks = min(maxlen, ptrs_per_blk - mp->mp_list[end_of_metadata]);
 		if (height == ip->i_height) {
 			 
 			iblks = height - sheight;
 			state = ALLOC_GROW_DEPTH;
 		} else {
 			 
 			state = ALLOC_GROW_HEIGHT;
 			iblks = height - ip->i_height;
 			branch_start = metapath_branch_start(mp);
 			iblks += (height - branch_start);
 		}
 	}
 
 	 
 
 	blks = dblks + iblks;
 	i = sheight;
 	do {
 		int error;
 		n = blks - alloced;
 		error = gfs2_alloc_block(ip, &bn, &n);
 		if (error)
 			return error;
 		alloced += n;
 		if (state != ALLOC_DATA || gfs2_is_jdata(ip))
 			gfs2_trans_add_unrevoke(sdp, bn, n);
 		switch (state) {
 		 
 		case ALLOC_GROW_HEIGHT:
 			if (i == 1) {
 				ptr = (__be64 *)(dibh->b_data +
 						 sizeof(struct gfs2_dinode));
 				zero_bn = *ptr;
 			}
 			for (; i - 1 < height - ip->i_height && n > 0; i++, n--)
 				gfs2_indirect_init(mp, ip->i_gl, i, 0, bn++);
 			if (i - 1 == height - ip->i_height) {
 				i--;
 				gfs2_buffer_copy_tail(mp->mp_bh[i],
 						sizeof(struct gfs2_meta_header),
 						dibh, sizeof(struct gfs2_dinode));
 				gfs2_buffer_clear_tail(dibh,
 						sizeof(struct gfs2_dinode) +
 						sizeof(__be64));
 				ptr = (__be64 *)(mp->mp_bh[i]->b_data +
 					sizeof(struct gfs2_meta_header));
 				*ptr = zero_bn;
 				state = ALLOC_GROW_DEPTH;
 				for(i = branch_start; i < height; i++) {
 					if (mp->mp_bh[i] == NULL)
 						break;
 					brelse(mp->mp_bh[i]);
 					mp->mp_bh[i] = NULL;
 				}
 				i = branch_start;
 			}
 			if (n == 0)
 				break;
 		 
 		case ALLOC_GROW_DEPTH:
 			if (i > 1 && i < height)
 				gfs2_trans_add_bh(ip->i_gl, mp->mp_bh[i-1], 1);
 			for (; i < height && n > 0; i++, n--)
 				gfs2_indirect_init(mp, ip->i_gl, i,
 						   mp->mp_list[i-1], bn++);
 			if (i == height)
 				state = ALLOC_DATA;
 			if (n == 0)
 				break;
 		 
 		case ALLOC_DATA:
 			BUG_ON(n > dblks);
 			BUG_ON(mp->mp_bh[end_of_metadata] == NULL);
 			gfs2_trans_add_bh(ip->i_gl, mp->mp_bh[end_of_metadata], 1);
 			dblks = n;
 			ptr = metapointer(end_of_metadata, mp);
  			dblock = bn;
  			while (n-- > 0)
  				*ptr++ = cpu_to_be64(bn++);
// 			if (buffer_zeronew(bh_map)) {
// 				ret = sb_issue_zeroout(sb, dblock, dblks,
// 						       GFP_NOFS);
// 				if (ret) {
// 					fs_err(sdp,
// 					       "Failed to zero data buffers\n");
// 					clear_buffer_zeronew(bh_map);
// 				}
// 			}
  			break;
  		}
  	} while ((state != ALLOC_DATA) || !dblock);
 
 	ip->i_height = height;
 	gfs2_add_inode_blocks(&ip->i_inode, alloced);
 	gfs2_dinode_out(ip, mp->mp_bh[0]->b_data);
 	map_bh(bh_map, inode->i_sb, dblock);
 	bh_map->b_size = dblks << inode->i_blkbits;
 	set_buffer_new(bh_map);
 	return 0;
 }