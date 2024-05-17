static int create_snapshot(struct btrfs_root *root, struct dentry *dentry,
			   char *name, int namelen, u64 *async_transid,
			   bool readonly, struct btrfs_qgroup_inherit **inherit)
{
	struct inode *inode;
	struct btrfs_pending_snapshot *pending_snapshot;
	struct btrfs_trans_handle *trans;
	int ret;

	if (!root->ref_cows)
		return -EINVAL;

	pending_snapshot = kzalloc(sizeof(*pending_snapshot), GFP_NOFS);
	if (!pending_snapshot)
		return -ENOMEM;

	btrfs_init_block_rsv(&pending_snapshot->block_rsv,
			     BTRFS_BLOCK_RSV_TEMP);
	pending_snapshot->dentry = dentry;
	pending_snapshot->root = root;
	pending_snapshot->readonly = readonly;
	if (inherit) {
		pending_snapshot->inherit = *inherit;
		*inherit = NULL;	 
	}

	trans = btrfs_start_transaction(root->fs_info->extent_root, 6);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
		goto fail;
	}

	ret = btrfs_snap_reserve_metadata(trans, pending_snapshot);
	BUG_ON(ret);

	spin_lock(&root->fs_info->trans_lock);
	list_add(&pending_snapshot->list,
		 &trans->transaction->pending_snapshots);
	spin_unlock(&root->fs_info->trans_lock);
	if (async_transid) {
		*async_transid = trans->transid;
		ret = btrfs_commit_transaction_async(trans,
				     root->fs_info->extent_root, 1);
	} else {
		ret = btrfs_commit_transaction(trans,
					       root->fs_info->extent_root);
	}
	if (ret) {
		 
		if (trans->aborted)
			pending_snapshot = NULL;
		goto fail;
	}

	ret = pending_snapshot->error;
	if (ret)
		goto fail;

	ret = btrfs_orphan_cleanup(pending_snapshot->snap);
	if (ret)
		goto fail;

	inode = btrfs_lookup_dentry(dentry->d_parent->d_inode, dentry);
	if (IS_ERR(inode)) {
		ret = PTR_ERR(inode);
		goto fail;
	}
	BUG_ON(!inode);
	d_instantiate(dentry, inode);
	ret = 0;
fail:
	kfree(pending_snapshot);
	return ret;
}