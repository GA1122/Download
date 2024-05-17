xfs_attr3_leaf_verify(
	struct xfs_buf			*bp)
{
	struct xfs_attr3_icleaf_hdr	ichdr;
	struct xfs_mount		*mp = bp->b_target->bt_mount;
	struct xfs_attr_leafblock	*leaf = bp->b_addr;
	struct xfs_perag		*pag = bp->b_pag;
	struct xfs_attr_leaf_entry	*entries;

	xfs_attr3_leaf_hdr_from_disk(mp->m_attr_geo, &ichdr, leaf);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_node_hdr *hdr3 = bp->b_addr;

		if (ichdr.magic != XFS_ATTR3_LEAF_MAGIC)
			return __this_address;

		if (!uuid_equal(&hdr3->info.uuid, &mp->m_sb.sb_meta_uuid))
			return __this_address;
		if (be64_to_cpu(hdr3->info.blkno) != bp->b_bn)
			return __this_address;
		if (!xfs_log_check_lsn(mp, be64_to_cpu(hdr3->info.lsn)))
			return __this_address;
	} else {
		if (ichdr.magic != XFS_ATTR_LEAF_MAGIC)
			return __this_address;
	}
	 
	if (pag && pag->pagf_init && ichdr.count == 0)
		return __this_address;

	 
	if (ichdr.firstused > mp->m_attr_geo->blksize)
		return __this_address;
	if (ichdr.firstused < xfs_attr3_leaf_hdr_size(leaf))
		return __this_address;

	 
	entries = xfs_attr3_leaf_entryp(bp->b_addr);
	if ((char *)&entries[ichdr.count] >
	    (char *)bp->b_addr + ichdr.firstused)
		return __this_address;

	 
	 

	return NULL;
}