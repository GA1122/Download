xfs_attr3_leaf_firstused_to_disk(
	struct xfs_da_geometry		*geo,
	struct xfs_attr_leafblock	*to,
	struct xfs_attr3_icleaf_hdr	*from)
{
	struct xfs_attr3_leaf_hdr	*hdr3;
	uint32_t			firstused;

	 
	ASSERT(from->firstused != XFS_ATTR3_LEAF_NULLOFF);

	 
	firstused = from->firstused;
	if (firstused > USHRT_MAX) {
		ASSERT(from->firstused == geo->blksize);
		firstused = XFS_ATTR3_LEAF_NULLOFF;
	}

	if (from->magic == XFS_ATTR3_LEAF_MAGIC) {
		hdr3 = (struct xfs_attr3_leaf_hdr *) to;
		hdr3->firstused = cpu_to_be16(firstused);
	} else {
		to->hdr.firstused = cpu_to_be16(firstused);
	}
}
