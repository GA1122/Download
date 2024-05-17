void btrfs_put_tree_mod_seq(struct btrfs_fs_info *fs_info,
			    struct seq_list *elem)
{
	struct rb_root *tm_root;
	struct rb_node *node;
	struct rb_node *next;
	struct seq_list *cur_elem;
	struct tree_mod_elem *tm;
	u64 min_seq = (u64)-1;
	u64 seq_putting = elem->seq;

	if (!seq_putting)
		return;

	spin_lock(&fs_info->tree_mod_seq_lock);
	list_del(&elem->list);
	elem->seq = 0;

	list_for_each_entry(cur_elem, &fs_info->tree_mod_seq_list, list) {
		if (cur_elem->seq < min_seq) {
			if (seq_putting > cur_elem->seq) {
				 
				spin_unlock(&fs_info->tree_mod_seq_lock);
				return;
			}
			min_seq = cur_elem->seq;
		}
	}
	spin_unlock(&fs_info->tree_mod_seq_lock);

	 
	tree_mod_log_write_lock(fs_info);
	tm_root = &fs_info->tree_mod_log;
	for (node = rb_first(tm_root); node; node = next) {
		next = rb_next(node);
		tm = container_of(node, struct tree_mod_elem, node);
		if (tm->seq > min_seq)
			continue;
		rb_erase(node, tm_root);
		kfree(tm);
	}
	tree_mod_log_write_unlock(fs_info);
}