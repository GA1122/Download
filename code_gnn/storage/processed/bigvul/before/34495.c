start_transaction(struct btrfs_root *root, u64 num_items, int type,
		  enum btrfs_reserve_flush_enum flush)
{
	struct btrfs_trans_handle *h;
	struct btrfs_transaction *cur_trans;
	u64 num_bytes = 0;
	int ret;
	u64 qgroup_reserved = 0;

	if (root->fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR)
		return ERR_PTR(-EROFS);

	if (current->journal_info) {
		WARN_ON(type != TRANS_JOIN && type != TRANS_JOIN_NOLOCK);
		h = current->journal_info;
		h->use_count++;
		WARN_ON(h->use_count > 2);
		h->orig_rsv = h->block_rsv;
		h->block_rsv = NULL;
		goto got_it;
	}

	 
	if (num_items > 0 && root != root->fs_info->chunk_root) {
		if (root->fs_info->quota_enabled &&
		    is_fstree(root->root_key.objectid)) {
			qgroup_reserved = num_items * root->leafsize;
			ret = btrfs_qgroup_reserve(root, qgroup_reserved);
			if (ret)
				return ERR_PTR(ret);
		}

		num_bytes = btrfs_calc_trans_metadata_size(root, num_items);
		ret = btrfs_block_rsv_add(root,
					  &root->fs_info->trans_block_rsv,
					  num_bytes, flush);
		if (ret)
			return ERR_PTR(ret);
	}
again:
	h = kmem_cache_alloc(btrfs_trans_handle_cachep, GFP_NOFS);
	if (!h)
		return ERR_PTR(-ENOMEM);

	 
	if (type < TRANS_JOIN_NOLOCK)
		sb_start_intwrite(root->fs_info->sb);

	if (may_wait_transaction(root, type))
		wait_current_trans(root);

	do {
		ret = join_transaction(root, type);
		if (ret == -EBUSY)
			wait_current_trans(root);
	} while (ret == -EBUSY);

	if (ret < 0) {
		 
		BUG_ON(type == TRANS_JOIN_NOLOCK);

		if (type < TRANS_JOIN_NOLOCK)
			sb_end_intwrite(root->fs_info->sb);
		kmem_cache_free(btrfs_trans_handle_cachep, h);
		return ERR_PTR(ret);
	}

	cur_trans = root->fs_info->running_transaction;

	h->transid = cur_trans->transid;
	h->transaction = cur_trans;
	h->blocks_used = 0;
	h->bytes_reserved = 0;
	h->root = root;
	h->delayed_ref_updates = 0;
	h->use_count = 1;
	h->adding_csums = 0;
	h->block_rsv = NULL;
	h->orig_rsv = NULL;
	h->aborted = 0;
	h->qgroup_reserved = qgroup_reserved;
	h->delayed_ref_elem.seq = 0;
	h->type = type;
	INIT_LIST_HEAD(&h->qgroup_ref_list);
	INIT_LIST_HEAD(&h->new_bgs);

	smp_mb();
	if (cur_trans->blocked && may_wait_transaction(root, type)) {
		btrfs_commit_transaction(h, root);
		goto again;
	}

	if (num_bytes) {
		trace_btrfs_space_reservation(root->fs_info, "transaction",
					      h->transid, num_bytes, 1);
		h->block_rsv = &root->fs_info->trans_block_rsv;
		h->bytes_reserved = num_bytes;
	}

got_it:
	btrfs_record_root_in_trans(h, root);

	if (!current->journal_info && type != TRANS_USERSPACE)
		current->journal_info = h;
	return h;
}