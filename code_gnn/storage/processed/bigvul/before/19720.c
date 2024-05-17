 void hugetlb_unreserve_pages(struct inode *inode, long offset, long freed)
{
	struct hstate *h = hstate_inode(inode);
	long chg = region_truncate(&inode->i_mapping->private_list, offset);
	struct hugepage_subpool *spool = subpool_inode(inode);

	spin_lock(&inode->i_lock);
	inode->i_blocks -= (blocks_per_huge_page(h) * freed);
	spin_unlock(&inode->i_lock);

	hugepage_subpool_put_pages(spool, (chg - freed));
	hugetlb_acct_memory(h, -(chg - freed));
}