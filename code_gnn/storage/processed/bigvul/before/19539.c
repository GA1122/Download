static noinline int udf_process_sequence(struct super_block *sb, long block,
				long lastblock, struct kernel_lb_addr *fileset)
{
	struct buffer_head *bh = NULL;
	struct udf_vds_record vds[VDS_POS_LENGTH];
	struct udf_vds_record *curr;
	struct generic_desc *gd;
	struct volDescPtr *vdp;
	int done = 0;
	uint32_t vdsn;
	uint16_t ident;
	long next_s = 0, next_e = 0;

	memset(vds, 0, sizeof(struct udf_vds_record) * VDS_POS_LENGTH);

	 
	for (; (!done && block <= lastblock); block++) {

		bh = udf_read_tagged(sb, block, block, &ident);
		if (!bh) {
			udf_err(sb,
				"Block %llu of volume descriptor sequence is corrupted or we could not read it\n",
				(unsigned long long)block);
			return 1;
		}

		 
		gd = (struct generic_desc *)bh->b_data;
		vdsn = le32_to_cpu(gd->volDescSeqNum);
		switch (ident) {
		case TAG_IDENT_PVD:  
			curr = &vds[VDS_POS_PRIMARY_VOL_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_VDP:  
			curr = &vds[VDS_POS_VOL_DESC_PTR];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;

				vdp = (struct volDescPtr *)bh->b_data;
				next_s = le32_to_cpu(
					vdp->nextVolDescSeqExt.extLocation);
				next_e = le32_to_cpu(
					vdp->nextVolDescSeqExt.extLength);
				next_e = next_e >> sb->s_blocksize_bits;
				next_e += next_s;
			}
			break;
		case TAG_IDENT_IUVD:  
			curr = &vds[VDS_POS_IMP_USE_VOL_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_PD:  
			curr = &vds[VDS_POS_PARTITION_DESC];
			if (!curr->block)
				curr->block = block;
			break;
		case TAG_IDENT_LVD:  
			curr = &vds[VDS_POS_LOGICAL_VOL_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_USD:  
			curr = &vds[VDS_POS_UNALLOC_SPACE_DESC];
			if (vdsn >= curr->volDescSeqNum) {
				curr->volDescSeqNum = vdsn;
				curr->block = block;
			}
			break;
		case TAG_IDENT_TD:  
			vds[VDS_POS_TERMINATING_DESC].block = block;
			if (next_e) {
				block = next_s;
				lastblock = next_e;
				next_s = next_e = 0;
			} else
				done = 1;
			break;
		}
		brelse(bh);
	}
	 
	if (!vds[VDS_POS_PRIMARY_VOL_DESC].block) {
		udf_err(sb, "Primary Volume Descriptor not found!\n");
		return 1;
	}
	if (udf_load_pvoldesc(sb, vds[VDS_POS_PRIMARY_VOL_DESC].block))
		return 1;

	if (vds[VDS_POS_LOGICAL_VOL_DESC].block && udf_load_logicalvol(sb,
	    vds[VDS_POS_LOGICAL_VOL_DESC].block, fileset))
		return 1;

	if (vds[VDS_POS_PARTITION_DESC].block) {
		 
		for (block = vds[VDS_POS_PARTITION_DESC].block;
		     block < vds[VDS_POS_TERMINATING_DESC].block;
		     block++)
			if (udf_load_partdesc(sb, block))
				return 1;
	}

	return 0;
}
