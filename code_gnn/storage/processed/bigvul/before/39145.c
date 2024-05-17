static int nfs_commit_unstable_pages(struct inode *inode, struct writeback_control *wbc)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	int flags = FLUSH_SYNC;
	int ret = 0;

	 
	if (!nfsi->commit_info.ncommit)
		return ret;

	if (wbc->sync_mode == WB_SYNC_NONE) {
		 
		if (nfsi->commit_info.ncommit <= (nfsi->npages >> 1))
			goto out_mark_dirty;

		 
		flags = 0;
	}

	ret = nfs_commit_inode(inode, flags);
	if (ret >= 0) {
		if (wbc->sync_mode == WB_SYNC_NONE) {
			if (ret < wbc->nr_to_write)
				wbc->nr_to_write -= ret;
			else
				wbc->nr_to_write = 0;
		}
		return 0;
	}
out_mark_dirty:
	__mark_inode_dirty(inode, I_DIRTY_DATASYNC);
	return ret;
}