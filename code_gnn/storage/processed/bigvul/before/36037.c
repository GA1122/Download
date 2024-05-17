static sector_t inode_getblk(struct inode *inode, sector_t block,
			     int *err, int *new)
{
	struct kernel_long_ad laarr[EXTENT_MERGE_SIZE];
	struct extent_position prev_epos, cur_epos, next_epos;
	int count = 0, startnum = 0, endnum = 0;
	uint32_t elen = 0, tmpelen;
	struct kernel_lb_addr eloc, tmpeloc;
	int c = 1;
	loff_t lbcount = 0, b_off = 0;
	uint32_t newblocknum, newblock;
	sector_t offset = 0;
	int8_t etype;
	struct udf_inode_info *iinfo = UDF_I(inode);
	int goal = 0, pgoal = iinfo->i_location.logicalBlockNum;
	int lastblock = 0;
	bool isBeyondEOF;

	*err = 0;
	*new = 0;
	prev_epos.offset = udf_file_entry_alloc_offset(inode);
	prev_epos.block = iinfo->i_location;
	prev_epos.bh = NULL;
	cur_epos = next_epos = prev_epos;
	b_off = (loff_t)block << inode->i_sb->s_blocksize_bits;

	 
	do {
		if (prev_epos.bh != cur_epos.bh) {
			brelse(prev_epos.bh);
			get_bh(cur_epos.bh);
			prev_epos.bh = cur_epos.bh;
		}
		if (cur_epos.bh != next_epos.bh) {
			brelse(cur_epos.bh);
			get_bh(next_epos.bh);
			cur_epos.bh = next_epos.bh;
		}

		lbcount += elen;

		prev_epos.block = cur_epos.block;
		cur_epos.block = next_epos.block;

		prev_epos.offset = cur_epos.offset;
		cur_epos.offset = next_epos.offset;

		etype = udf_next_aext(inode, &next_epos, &eloc, &elen, 1);
		if (etype == -1)
			break;

		c = !c;

		laarr[c].extLength = (etype << 30) | elen;
		laarr[c].extLocation = eloc;

		if (etype != (EXT_NOT_RECORDED_NOT_ALLOCATED >> 30))
			pgoal = eloc.logicalBlockNum +
				((elen + inode->i_sb->s_blocksize - 1) >>
				 inode->i_sb->s_blocksize_bits);

		count++;
	} while (lbcount + elen <= b_off);

	b_off -= lbcount;
	offset = b_off >> inode->i_sb->s_blocksize_bits;
	 
	udf_next_aext(inode, &prev_epos, &tmpeloc, &tmpelen, 0);
	udf_next_aext(inode, &cur_epos, &tmpeloc, &tmpelen, 0);

	 

	if (etype == (EXT_RECORDED_ALLOCATED >> 30)) {
		if (elen & (inode->i_sb->s_blocksize - 1)) {
			elen = EXT_RECORDED_ALLOCATED |
				((elen + inode->i_sb->s_blocksize - 1) &
				 ~(inode->i_sb->s_blocksize - 1));
			udf_write_aext(inode, &cur_epos, &eloc, elen, 1);
		}
		brelse(prev_epos.bh);
		brelse(cur_epos.bh);
		brelse(next_epos.bh);
		newblock = udf_get_lb_pblock(inode->i_sb, &eloc, offset);
		return newblock;
	}

	 
	if (etype == -1) {
		int ret;
		isBeyondEOF = 1;
		if (count) {
			if (c)
				laarr[0] = laarr[1];
			startnum = 1;
		} else {
			 
			memset(&laarr[0].extLocation, 0x00,
				sizeof(struct kernel_lb_addr));
			laarr[0].extLength = EXT_NOT_RECORDED_NOT_ALLOCATED;
			 
			startnum = (offset > 0);
		}
		 
		ret = udf_do_extend_file(inode, &prev_epos, laarr, offset);
		if (ret < 0) {
			brelse(prev_epos.bh);
			brelse(cur_epos.bh);
			brelse(next_epos.bh);
			*err = ret;
			return 0;
		}
		c = 0;
		offset = 0;
		count += ret;
		 
		if ((laarr[0].extLength & UDF_EXTENT_FLAG_MASK) !=
						EXT_NOT_RECORDED_ALLOCATED) {
			 
			if (count)
				c = !c;
			laarr[c].extLength = EXT_NOT_RECORDED_NOT_ALLOCATED |
				inode->i_sb->s_blocksize;
			memset(&laarr[c].extLocation, 0x00,
				sizeof(struct kernel_lb_addr));
			count++;
		}
		endnum = c + 1;
		lastblock = 1;
	} else {
		isBeyondEOF = 0;
		endnum = startnum = ((count > 2) ? 2 : count);

		 
		if (!c && count != 1) {
			laarr[2] = laarr[0];
			laarr[0] = laarr[1];
			laarr[1] = laarr[2];
			c = 1;
		}

		 
		etype = udf_next_aext(inode, &next_epos, &eloc, &elen, 0);
		if (etype != -1) {
			laarr[c + 1].extLength = (etype << 30) | elen;
			laarr[c + 1].extLocation = eloc;
			count++;
			startnum++;
			endnum++;
		} else
			lastblock = 1;
	}

	 
	if ((laarr[c].extLength >> 30) == (EXT_NOT_RECORDED_ALLOCATED >> 30))
		newblocknum = laarr[c].extLocation.logicalBlockNum + offset;
	else {  
		if (iinfo->i_next_alloc_block == block)
			goal = iinfo->i_next_alloc_goal;

		if (!goal) {
			if (!(goal = pgoal))  
				goal = iinfo->i_location.logicalBlockNum + 1;
		}

		newblocknum = udf_new_block(inode->i_sb, inode,
				iinfo->i_location.partitionReferenceNum,
				goal, err);
		if (!newblocknum) {
			brelse(prev_epos.bh);
			brelse(cur_epos.bh);
			brelse(next_epos.bh);
			*err = -ENOSPC;
			return 0;
		}
		if (isBeyondEOF)
			iinfo->i_lenExtents += inode->i_sb->s_blocksize;
	}

	 
	udf_split_extents(inode, &c, offset, newblocknum, laarr, &endnum);

#ifdef UDF_PREALLOCATE
	 
	if (S_ISREG(inode->i_mode))
		udf_prealloc_extents(inode, c, lastblock, laarr, &endnum);
#endif

	 
	udf_merge_extents(inode, laarr, &endnum);

	 
	udf_update_extents(inode, laarr, startnum, endnum, &prev_epos);

	brelse(prev_epos.bh);
	brelse(cur_epos.bh);
	brelse(next_epos.bh);

	newblock = udf_get_pblock(inode->i_sb, newblocknum,
				iinfo->i_location.partitionReferenceNum, 0);
	if (!newblock) {
		*err = -EIO;
		return 0;
	}
	*new = 1;
	iinfo->i_next_alloc_block = block;
	iinfo->i_next_alloc_goal = newblocknum;
	inode->i_ctime = current_fs_time(inode->i_sb);

	if (IS_SYNC(inode))
		udf_sync_inode(inode);
	else
		mark_inode_dirty(inode);

	return newblock;
}
