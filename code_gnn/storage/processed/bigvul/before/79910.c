xfs_attr3_leaf_compact(
	struct xfs_da_args	*args,
	struct xfs_attr3_icleaf_hdr *ichdr_dst,
	struct xfs_buf		*bp)
{
	struct xfs_attr_leafblock *leaf_src;
	struct xfs_attr_leafblock *leaf_dst;
	struct xfs_attr3_icleaf_hdr ichdr_src;
	struct xfs_trans	*trans = args->trans;
	char			*tmpbuffer;

	trace_xfs_attr_leaf_compact(args);

	tmpbuffer = kmem_alloc(args->geo->blksize, KM_SLEEP);
	memcpy(tmpbuffer, bp->b_addr, args->geo->blksize);
	memset(bp->b_addr, 0, args->geo->blksize);
	leaf_src = (xfs_attr_leafblock_t *)tmpbuffer;
	leaf_dst = bp->b_addr;

	 
	memcpy(bp->b_addr, tmpbuffer, xfs_attr3_leaf_hdr_size(leaf_src));

	 
	ichdr_src = *ichdr_dst;	 
	ichdr_dst->firstused = args->geo->blksize;
	ichdr_dst->usedbytes = 0;
	ichdr_dst->count = 0;
	ichdr_dst->holes = 0;
	ichdr_dst->freemap[0].base = xfs_attr3_leaf_hdr_size(leaf_src);
	ichdr_dst->freemap[0].size = ichdr_dst->firstused -
						ichdr_dst->freemap[0].base;

	 
	xfs_attr3_leaf_hdr_to_disk(args->geo, leaf_dst, ichdr_dst);

	 
	xfs_attr3_leaf_moveents(args, leaf_src, &ichdr_src, 0,
				leaf_dst, ichdr_dst, 0, ichdr_src.count);
	 
	xfs_trans_log_buf(trans, bp, 0, args->geo->blksize - 1);

	kmem_free(tmpbuffer);
}
