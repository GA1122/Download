xfs_buf_is_vmapped(
	struct xfs_buf	*bp)
{
	 
	return bp->b_addr && bp->b_page_count > 1;
}
