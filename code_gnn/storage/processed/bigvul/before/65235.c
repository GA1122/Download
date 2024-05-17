void nlmsvc_traverse_blocks(struct nlm_host *host,
			struct nlm_file *file,
			nlm_host_match_fn_t match)
{
	struct nlm_block *block, *next;

restart:
	mutex_lock(&file->f_mutex);
	list_for_each_entry_safe(block, next, &file->f_blocks, b_flist) {
		if (!match(block->b_host, host))
			continue;
		 
		if (list_empty(&block->b_list))
			continue;
		kref_get(&block->b_count);
		mutex_unlock(&file->f_mutex);
		nlmsvc_unlink_block(block);
		nlmsvc_release_block(block);
		goto restart;
	}
	mutex_unlock(&file->f_mutex);
}