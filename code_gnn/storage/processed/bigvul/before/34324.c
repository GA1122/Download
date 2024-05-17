int btrfs_orphan_cleanup(struct btrfs_root *root)
{
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_key key, found_key;
	struct btrfs_trans_handle *trans;
	struct inode *inode;
	u64 last_objectid = 0;
	int ret = 0, nr_unlink = 0, nr_truncate = 0;

	if (cmpxchg(&root->orphan_cleanup_state, 0, ORPHAN_CLEANUP_STARTED))
		return 0;

	path = btrfs_alloc_path();
	if (!path) {
		ret = -ENOMEM;
		goto out;
	}
	path->reada = -1;

	key.objectid = BTRFS_ORPHAN_OBJECTID;
	btrfs_set_key_type(&key, BTRFS_ORPHAN_ITEM_KEY);
	key.offset = (u64)-1;

	while (1) {
		ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
		if (ret < 0)
			goto out;

		 
		if (ret > 0) {
			ret = 0;
			if (path->slots[0] == 0)
				break;
			path->slots[0]--;
		}

		 
		leaf = path->nodes[0];
		btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);

		 
		if (found_key.objectid != BTRFS_ORPHAN_OBJECTID)
			break;
		if (btrfs_key_type(&found_key) != BTRFS_ORPHAN_ITEM_KEY)
			break;

		 
		btrfs_release_path(path);

		 

		if (found_key.offset == last_objectid) {
			printk(KERN_ERR "btrfs: Error removing orphan entry, "
			       "stopping orphan cleanup\n");
			ret = -EINVAL;
			goto out;
		}

		last_objectid = found_key.offset;

		found_key.objectid = found_key.offset;
		found_key.type = BTRFS_INODE_ITEM_KEY;
		found_key.offset = 0;
		inode = btrfs_iget(root->fs_info->sb, &found_key, root, NULL);
		ret = PTR_RET(inode);
		if (ret && ret != -ESTALE)
			goto out;

		if (ret == -ESTALE && root == root->fs_info->tree_root) {
			struct btrfs_root *dead_root;
			struct btrfs_fs_info *fs_info = root->fs_info;
			int is_dead_root = 0;

			 
			spin_lock(&fs_info->trans_lock);
			list_for_each_entry(dead_root, &fs_info->dead_roots,
					    root_list) {
				if (dead_root->root_key.objectid ==
				    found_key.objectid) {
					is_dead_root = 1;
					break;
				}
			}
			spin_unlock(&fs_info->trans_lock);
			if (is_dead_root) {
				 
				key.offset = found_key.objectid - 1;
				continue;
			}
		}
		 
		if (ret == -ESTALE) {
			trans = btrfs_start_transaction(root, 1);
			if (IS_ERR(trans)) {
				ret = PTR_ERR(trans);
				goto out;
			}
			printk(KERN_ERR "auto deleting %Lu\n",
			       found_key.objectid);
			ret = btrfs_del_orphan_item(trans, root,
						    found_key.objectid);
			BUG_ON(ret);  
			btrfs_end_transaction(trans, root);
			continue;
		}

		 
		set_bit(BTRFS_INODE_HAS_ORPHAN_ITEM,
			&BTRFS_I(inode)->runtime_flags);

		 
		if (inode->i_nlink) {
			if (!S_ISREG(inode->i_mode)) {
				WARN_ON(1);
				iput(inode);
				continue;
			}
			nr_truncate++;
			ret = btrfs_truncate(inode);
		} else {
			nr_unlink++;
		}

		 
		iput(inode);
		if (ret)
			goto out;
	}
	 
	btrfs_release_path(path);

	root->orphan_cleanup_state = ORPHAN_CLEANUP_DONE;

	if (root->orphan_block_rsv)
		btrfs_block_rsv_release(root, root->orphan_block_rsv,
					(u64)-1);

	if (root->orphan_block_rsv || root->orphan_item_inserted) {
		trans = btrfs_join_transaction(root);
		if (!IS_ERR(trans))
			btrfs_end_transaction(trans, root);
	}

	if (nr_unlink)
		printk(KERN_INFO "btrfs: unlinked %d orphans\n", nr_unlink);
	if (nr_truncate)
		printk(KERN_INFO "btrfs: truncated %d orphans\n", nr_truncate);

out:
	if (ret)
		printk(KERN_CRIT "btrfs: could not do orphan cleanup %d\n", ret);
	btrfs_free_path(path);
	return ret;
}