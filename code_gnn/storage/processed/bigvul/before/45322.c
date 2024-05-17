int btrfs_search_old_slot(struct btrfs_root *root, struct btrfs_key *key,
			  struct btrfs_path *p, u64 time_seq)
{
	struct extent_buffer *b;
	int slot;
	int ret;
	int err;
	int level;
	int lowest_unlock = 1;
	u8 lowest_level = 0;
	int prev_cmp = -1;

	lowest_level = p->lowest_level;
	WARN_ON(p->nodes[0] != NULL);

	if (p->search_commit_root) {
		BUG_ON(time_seq);
		return btrfs_search_slot(NULL, root, key, p, 0, 0);
	}

again:
	b = get_old_root(root, time_seq);
	level = btrfs_header_level(b);
	p->locks[level] = BTRFS_READ_LOCK;

	while (b) {
		level = btrfs_header_level(b);
		p->nodes[level] = b;
		btrfs_clear_path_blocking(p, NULL, 0);

		 
		btrfs_unlock_up_safe(p, level + 1);

		 
		prev_cmp = -1;
		ret = key_search(b, key, level, &prev_cmp, &slot);

		if (level != 0) {
			int dec = 0;
			if (ret && slot > 0) {
				dec = 1;
				slot -= 1;
			}
			p->slots[level] = slot;
			unlock_up(p, level, lowest_unlock, 0, NULL);

			if (level == lowest_level) {
				if (dec)
					p->slots[level]++;
				goto done;
			}

			err = read_block_for_search(NULL, root, p, &b, level,
						    slot, key, time_seq);
			if (err == -EAGAIN)
				goto again;
			if (err) {
				ret = err;
				goto done;
			}

			level = btrfs_header_level(b);
			err = btrfs_try_tree_read_lock(b);
			if (!err) {
				btrfs_set_path_blocking(p);
				btrfs_tree_read_lock(b);
				btrfs_clear_path_blocking(p, b,
							  BTRFS_READ_LOCK);
			}
			b = tree_mod_log_rewind(root->fs_info, p, b, time_seq);
			if (!b) {
				ret = -ENOMEM;
				goto done;
			}
			p->locks[level] = BTRFS_READ_LOCK;
			p->nodes[level] = b;
		} else {
			p->slots[level] = slot;
			unlock_up(p, level, lowest_unlock, 0, NULL);
			goto done;
		}
	}
	ret = 1;
done:
	if (!p->leave_spinning)
		btrfs_set_path_blocking(p);
	if (ret < 0)
		btrfs_release_path(p);

	return ret;
}