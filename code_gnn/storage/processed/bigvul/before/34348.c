int btrfs_start_delalloc_inodes(struct btrfs_root *root, int delay_iput)
{
	struct list_head *head = &root->fs_info->delalloc_inodes;
	struct btrfs_inode *binode;
	struct inode *inode;
	struct btrfs_delalloc_work *work, *next;
	struct list_head works;
	int ret = 0;

	if (root->fs_info->sb->s_flags & MS_RDONLY)
		return -EROFS;

	INIT_LIST_HEAD(&works);

	spin_lock(&root->fs_info->delalloc_lock);
	while (!list_empty(head)) {
		binode = list_entry(head->next, struct btrfs_inode,
				    delalloc_inodes);
		inode = igrab(&binode->vfs_inode);
		if (!inode)
			list_del_init(&binode->delalloc_inodes);
		spin_unlock(&root->fs_info->delalloc_lock);
		if (inode) {
			work = btrfs_alloc_delalloc_work(inode, 0, delay_iput);
			if (!work) {
				ret = -ENOMEM;
				goto out;
			}
			list_add_tail(&work->list, &works);
			btrfs_queue_worker(&root->fs_info->flush_workers,
					   &work->work);
		}
		cond_resched();
		spin_lock(&root->fs_info->delalloc_lock);
	}
	spin_unlock(&root->fs_info->delalloc_lock);

	 
	atomic_inc(&root->fs_info->async_submit_draining);
	while (atomic_read(&root->fs_info->nr_async_submits) ||
	      atomic_read(&root->fs_info->async_delalloc_pages)) {
		wait_event(root->fs_info->async_submit_wait,
		   (atomic_read(&root->fs_info->nr_async_submits) == 0 &&
		    atomic_read(&root->fs_info->async_delalloc_pages) == 0));
	}
	atomic_dec(&root->fs_info->async_submit_draining);
out:
	list_for_each_entry_safe(work, next, &works, list) {
		list_del_init(&work->list);
		btrfs_wait_and_free_delalloc_work(work);
	}
	return ret;
}