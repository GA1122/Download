xfs_attr3_rmt_hdr_ok(
	struct xfs_mount	*mp,
	void			*ptr,
	xfs_ino_t		ino,
	uint32_t		offset,
	uint32_t		size,
	xfs_daddr_t		bno)
{
	struct xfs_attr3_rmt_hdr *rmt = ptr;

	if (bno != be64_to_cpu(rmt->rm_blkno))
		return false;
	if (offset != be32_to_cpu(rmt->rm_offset))
		return false;
	if (size != be32_to_cpu(rmt->rm_bytes))
		return false;
	if (ino != be64_to_cpu(rmt->rm_owner))
		return false;

	 
	return true;
}