static noinline int create_subvol(struct btrfs_root *root,
				  struct dentry *dentry,
				  char *name, int namelen,
				  u64 *async_transid,
				  struct btrfs_qgroup_inherit **inherit)
{
	struct btrfs_trans_handle *trans;
	struct btrfs_key key;
	struct btrfs_root_item root_item;
	struct btrfs_inode_item *inode_item;
	struct extent_buffer *leaf;
	struct btrfs_root *new_root;
	struct dentry *parent = dentry->d_parent;
	struct inode *dir;
	struct timespec cur_time = CURRENT_TIME;
	int ret;
	int err;
	u64 objectid;
	u64 new_dirid = BTRFS_FIRST_FREE_OBJECTID;
	u64 index = 0;
	uuid_le new_uuid;

	ret = btrfs_find_free_objectid(root->fs_info->tree_root, &objectid);
	if (ret)
		return ret;

	dir = parent->d_inode;

	 
	trans = btrfs_start_transaction(root, 6);
	if (IS_ERR(trans))
		return PTR_ERR(trans);

	ret = btrfs_qgroup_inherit(trans, root->fs_info, 0, objectid,
				   inherit ? *inherit : NULL);
	if (ret)
		goto fail;

	leaf = btrfs_alloc_free_block(trans, root, root->leafsize,
				      0, objectid, NULL, 0, 0, 0);
	if (IS_ERR(leaf)) {
		ret = PTR_ERR(leaf);
		goto fail;
	}

	memset_extent_buffer(leaf, 0, 0, sizeof(struct btrfs_header));
	btrfs_set_header_bytenr(leaf, leaf->start);
	btrfs_set_header_generation(leaf, trans->transid);
	btrfs_set_header_backref_rev(leaf, BTRFS_MIXED_BACKREF_REV);
	btrfs_set_header_owner(leaf, objectid);

	write_extent_buffer(leaf, root->fs_info->fsid,
			    (unsigned long)btrfs_header_fsid(leaf),
			    BTRFS_FSID_SIZE);
	write_extent_buffer(leaf, root->fs_info->chunk_tree_uuid,
			    (unsigned long)btrfs_header_chunk_tree_uuid(leaf),
			    BTRFS_UUID_SIZE);
	btrfs_mark_buffer_dirty(leaf);

	memset(&root_item, 0, sizeof(root_item));

	inode_item = &root_item.inode;
	inode_item->generation = cpu_to_le64(1);
	inode_item->size = cpu_to_le64(3);
	inode_item->nlink = cpu_to_le32(1);
	inode_item->nbytes = cpu_to_le64(root->leafsize);
	inode_item->mode = cpu_to_le32(S_IFDIR | 0755);

	root_item.flags = 0;
	root_item.byte_limit = 0;
	inode_item->flags = cpu_to_le64(BTRFS_INODE_ROOT_ITEM_INIT);

	btrfs_set_root_bytenr(&root_item, leaf->start);
	btrfs_set_root_generation(&root_item, trans->transid);
	btrfs_set_root_level(&root_item, 0);
	btrfs_set_root_refs(&root_item, 1);
	btrfs_set_root_used(&root_item, leaf->len);
	btrfs_set_root_last_snapshot(&root_item, 0);

	btrfs_set_root_generation_v2(&root_item,
			btrfs_root_generation(&root_item));
	uuid_le_gen(&new_uuid);
	memcpy(root_item.uuid, new_uuid.b, BTRFS_UUID_SIZE);
	root_item.otime.sec = cpu_to_le64(cur_time.tv_sec);
	root_item.otime.nsec = cpu_to_le32(cur_time.tv_nsec);
	root_item.ctime = root_item.otime;
	btrfs_set_root_ctransid(&root_item, trans->transid);
	btrfs_set_root_otransid(&root_item, trans->transid);

	btrfs_tree_unlock(leaf);
	free_extent_buffer(leaf);
	leaf = NULL;

	btrfs_set_root_dirid(&root_item, new_dirid);

	key.objectid = objectid;
	key.offset = 0;
	btrfs_set_key_type(&key, BTRFS_ROOT_ITEM_KEY);
	ret = btrfs_insert_root(trans, root->fs_info->tree_root, &key,
				&root_item);
	if (ret)
		goto fail;

	key.offset = (u64)-1;
	new_root = btrfs_read_fs_root_no_name(root->fs_info, &key);
	if (IS_ERR(new_root)) {
		btrfs_abort_transaction(trans, root, PTR_ERR(new_root));
		ret = PTR_ERR(new_root);
		goto fail;
	}

	btrfs_record_root_in_trans(trans, new_root);

	ret = btrfs_create_subvol_root(trans, new_root, new_dirid);
	if (ret) {
		 
		btrfs_abort_transaction(trans, root, ret);
		goto fail;
	}

	 
	ret = btrfs_set_inode_index(dir, &index);
	if (ret) {
		btrfs_abort_transaction(trans, root, ret);
		goto fail;
	}

	ret = btrfs_insert_dir_item(trans, root,
				    name, namelen, dir, &key,
				    BTRFS_FT_DIR, index);
	if (ret) {
		btrfs_abort_transaction(trans, root, ret);
		goto fail;
	}

	btrfs_i_size_write(dir, dir->i_size + namelen * 2);
	ret = btrfs_update_inode(trans, root, dir);
	BUG_ON(ret);

	ret = btrfs_add_root_ref(trans, root->fs_info->tree_root,
				 objectid, root->root_key.objectid,
				 btrfs_ino(dir), index, name, namelen);

	BUG_ON(ret);

	d_instantiate(dentry, btrfs_lookup_dentry(dir, dentry));
fail:
	if (async_transid) {
		*async_transid = trans->transid;
		err = btrfs_commit_transaction_async(trans, root, 1);
	} else {
		err = btrfs_commit_transaction(trans, root);
	}
	if (err && !ret)
		ret = err;
	return ret;
}