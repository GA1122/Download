 static noinline int create_pending_snapshot(struct btrfs_trans_handle *trans,
 				   struct btrfs_fs_info *fs_info,
 				   struct btrfs_pending_snapshot *pending)
 {
 	struct btrfs_key key;
 	struct btrfs_root_item *new_root_item;
 	struct btrfs_root *tree_root = fs_info->tree_root;
 	struct btrfs_root *root = pending->root;
 	struct btrfs_root *parent_root;
 	struct btrfs_block_rsv *rsv;
 	struct inode *parent_inode;
 	struct btrfs_path *path;
 	struct btrfs_dir_item *dir_item;
 	struct dentry *parent;
 	struct dentry *dentry;
 	struct extent_buffer *tmp;
 	struct extent_buffer *old;
 	struct timespec cur_time = CURRENT_TIME;
 	int ret;
 	u64 to_reserve = 0;
 	u64 index = 0;
 	u64 objectid;
 	u64 root_flags;
 	uuid_le new_uuid;
 
 	path = btrfs_alloc_path();
 	if (!path) {
 		ret = pending->error = -ENOMEM;
 		goto path_alloc_fail;
 	}
 
 	new_root_item = kmalloc(sizeof(*new_root_item), GFP_NOFS);
 	if (!new_root_item) {
 		ret = pending->error = -ENOMEM;
 		goto root_item_alloc_fail;
 	}
 
 	ret = btrfs_find_free_objectid(tree_root, &objectid);
 	if (ret) {
 		pending->error = ret;
 		goto no_free_objectid;
 	}
 
 	btrfs_reloc_pre_snapshot(trans, pending, &to_reserve);
 
 	if (to_reserve > 0) {
 		ret = btrfs_block_rsv_add(root, &pending->block_rsv,
 					  to_reserve,
 					  BTRFS_RESERVE_NO_FLUSH);
 		if (ret) {
 			pending->error = ret;
 			goto no_free_objectid;
 		}
 	}
 
 	ret = btrfs_qgroup_inherit(trans, fs_info, root->root_key.objectid,
 				   objectid, pending->inherit);
 	if (ret) {
 		pending->error = ret;
 		goto no_free_objectid;
 	}
 
 	key.objectid = objectid;
 	key.offset = (u64)-1;
 	key.type = BTRFS_ROOT_ITEM_KEY;
 
 	rsv = trans->block_rsv;
 	trans->block_rsv = &pending->block_rsv;
 
 	dentry = pending->dentry;
 	parent = dget_parent(dentry);
 	parent_inode = parent->d_inode;
 	parent_root = BTRFS_I(parent_inode)->root;
 	record_root_in_trans(trans, parent_root);
 
 	 
 	ret = btrfs_set_inode_index(parent_inode, &index);
 	BUG_ON(ret);  
 
 	 
 	dir_item = btrfs_lookup_dir_item(NULL, parent_root, path,
 					 btrfs_ino(parent_inode),
 					 dentry->d_name.name,
 					 dentry->d_name.len, 0);
 	if (dir_item != NULL && !IS_ERR(dir_item)) {
 		pending->error = -EEXIST;
 		goto fail;
 	} else if (IS_ERR(dir_item)) {
 		ret = PTR_ERR(dir_item);
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 	btrfs_release_path(path);
 
 	 
 	ret = btrfs_run_delayed_items(trans, root);
 	if (ret) {	 
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	record_root_in_trans(trans, root);
 	btrfs_set_root_last_snapshot(&root->root_item, trans->transid);
 	memcpy(new_root_item, &root->root_item, sizeof(*new_root_item));
 	btrfs_check_and_init_root_item(new_root_item);
 
 	root_flags = btrfs_root_flags(new_root_item);
 	if (pending->readonly)
 		root_flags |= BTRFS_ROOT_SUBVOL_RDONLY;
 	else
 		root_flags &= ~BTRFS_ROOT_SUBVOL_RDONLY;
 	btrfs_set_root_flags(new_root_item, root_flags);
 
 	btrfs_set_root_generation_v2(new_root_item,
 			trans->transid);
 	uuid_le_gen(&new_uuid);
 	memcpy(new_root_item->uuid, new_uuid.b, BTRFS_UUID_SIZE);
 	memcpy(new_root_item->parent_uuid, root->root_item.uuid,
 			BTRFS_UUID_SIZE);
 	new_root_item->otime.sec = cpu_to_le64(cur_time.tv_sec);
 	new_root_item->otime.nsec = cpu_to_le32(cur_time.tv_nsec);
 	btrfs_set_root_otransid(new_root_item, trans->transid);
 	memset(&new_root_item->stime, 0, sizeof(new_root_item->stime));
 	memset(&new_root_item->rtime, 0, sizeof(new_root_item->rtime));
 	btrfs_set_root_stransid(new_root_item, 0);
 	btrfs_set_root_rtransid(new_root_item, 0);
 
 	old = btrfs_lock_root_node(root);
 	ret = btrfs_cow_block(trans, root, old, NULL, 0, &old);
 	if (ret) {
 		btrfs_tree_unlock(old);
 		free_extent_buffer(old);
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	btrfs_set_lock_blocking(old);
 
 	ret = btrfs_copy_root(trans, root, old, &tmp, objectid);
 	 
 	btrfs_tree_unlock(old);
 	free_extent_buffer(old);
 	if (ret) {
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	 
 	root->force_cow = 1;
 	smp_wmb();
 
 	btrfs_set_root_node(new_root_item, tmp);
 	 
 	key.offset = trans->transid;
 	ret = btrfs_insert_root(trans, tree_root, &key, new_root_item);
 	btrfs_tree_unlock(tmp);
 	free_extent_buffer(tmp);
 	if (ret) {
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	 
 	ret = btrfs_add_root_ref(trans, tree_root, objectid,
 				 parent_root->root_key.objectid,
 				 btrfs_ino(parent_inode), index,
 				 dentry->d_name.name, dentry->d_name.len);
 	if (ret) {
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	key.offset = (u64)-1;
 	pending->snap = btrfs_read_fs_root_no_name(root->fs_info, &key);
 	if (IS_ERR(pending->snap)) {
 		ret = PTR_ERR(pending->snap);
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	ret = btrfs_reloc_post_snapshot(trans, pending);
 	if (ret) {
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	ret = btrfs_run_delayed_refs(trans, root, (unsigned long)-1);
 	if (ret) {
 		btrfs_abort_transaction(trans, root, ret);
 		goto fail;
 	}
 
 	ret = btrfs_insert_dir_item(trans, parent_root,
 				    dentry->d_name.name, dentry->d_name.len,
  				    parent_inode, &key,
  				    BTRFS_FT_DIR, index);
  	 
	BUG_ON(ret == -EEXIST);
// 	BUG_ON(ret == -EEXIST || ret == -EOVERFLOW);
  	if (ret) {
  		btrfs_abort_transaction(trans, root, ret);
  		goto fail;
 	}
 
 	btrfs_i_size_write(parent_inode, parent_inode->i_size +
 					 dentry->d_name.len * 2);
 	parent_inode->i_mtime = parent_inode->i_ctime = CURRENT_TIME;
 	ret = btrfs_update_inode_fallback(trans, parent_root, parent_inode);
 	if (ret)
 		btrfs_abort_transaction(trans, root, ret);
 fail:
 	dput(parent);
 	trans->block_rsv = rsv;
 no_free_objectid:
 	kfree(new_root_item);
 root_item_alloc_fail:
 	btrfs_free_path(path);
 path_alloc_fail:
 	btrfs_block_rsv_release(root, &pending->block_rsv, (u64)-1);
 	return ret;
 }