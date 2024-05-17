tree_mod_log_insert_move(struct btrfs_fs_info *fs_info,
			 struct extent_buffer *eb, int dst_slot, int src_slot,
			 int nr_items, gfp_t flags)
{
	struct tree_mod_elem *tm = NULL;
	struct tree_mod_elem **tm_list = NULL;
	int ret = 0;
	int i;
	int locked = 0;

	if (!tree_mod_need_log(fs_info, eb))
		return 0;

	tm_list = kzalloc(nr_items * sizeof(struct tree_mod_elem *), flags);
	if (!tm_list)
		return -ENOMEM;

	tm = kzalloc(sizeof(*tm), flags);
	if (!tm) {
		ret = -ENOMEM;
		goto free_tms;
	}

	tm->index = eb->start >> PAGE_CACHE_SHIFT;
	tm->slot = src_slot;
	tm->move.dst_slot = dst_slot;
	tm->move.nr_items = nr_items;
	tm->op = MOD_LOG_MOVE_KEYS;

	for (i = 0; i + dst_slot < src_slot && i < nr_items; i++) {
		tm_list[i] = alloc_tree_mod_elem(eb, i + dst_slot,
		    MOD_LOG_KEY_REMOVE_WHILE_MOVING, flags);
		if (!tm_list[i]) {
			ret = -ENOMEM;
			goto free_tms;
		}
	}

	if (tree_mod_dont_log(fs_info, eb))
		goto free_tms;
	locked = 1;

	 
	for (i = 0; i + dst_slot < src_slot && i < nr_items; i++) {
		ret = __tree_mod_log_insert(fs_info, tm_list[i]);
		if (ret)
			goto free_tms;
	}

	ret = __tree_mod_log_insert(fs_info, tm);
	if (ret)
		goto free_tms;
	tree_mod_log_write_unlock(fs_info);
	kfree(tm_list);

	return 0;
free_tms:
	for (i = 0; i < nr_items; i++) {
		if (tm_list[i] && !RB_EMPTY_NODE(&tm_list[i]->node))
			rb_erase(&tm_list[i]->node, &fs_info->tree_mod_log);
		kfree(tm_list[i]);
	}
	if (locked)
		tree_mod_log_write_unlock(fs_info);
	kfree(tm_list);
	kfree(tm);

	return ret;
}