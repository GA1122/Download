static noinline long btrfs_ioctl_clone(struct file *file, unsigned long srcfd,
				       u64 off, u64 olen, u64 destoff)
{
	struct inode *inode = fdentry(file)->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct fd src_file;
	struct inode *src;
	struct btrfs_trans_handle *trans;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	char *buf;
	struct btrfs_key key;
	u32 nritems;
	int slot;
	int ret;
	u64 len = olen;
	u64 bs = root->fs_info->sb->s_blocksize;

	 

	 
	if (!(file->f_mode & FMODE_WRITE) || (file->f_flags & O_APPEND))
		return -EINVAL;

	if (btrfs_root_readonly(root))
		return -EROFS;

	ret = mnt_want_write_file(file);
	if (ret)
		return ret;

	src_file = fdget(srcfd);
	if (!src_file.file) {
		ret = -EBADF;
		goto out_drop_write;
	}

	ret = -EXDEV;
	if (src_file.file->f_path.mnt != file->f_path.mnt)
		goto out_fput;

	src = src_file.file->f_dentry->d_inode;

	ret = -EINVAL;
	if (src == inode)
		goto out_fput;

	 
	if (!(src_file.file->f_mode & FMODE_READ))
		goto out_fput;

	 
	if ((BTRFS_I(src)->flags & BTRFS_INODE_NODATASUM) !=
	    (BTRFS_I(inode)->flags & BTRFS_INODE_NODATASUM))
		goto out_fput;

	ret = -EISDIR;
	if (S_ISDIR(src->i_mode) || S_ISDIR(inode->i_mode))
		goto out_fput;

	ret = -EXDEV;
	if (src->i_sb != inode->i_sb)
		goto out_fput;

	ret = -ENOMEM;
	buf = vmalloc(btrfs_level_size(root, 0));
	if (!buf)
		goto out_fput;

	path = btrfs_alloc_path();
	if (!path) {
		vfree(buf);
		goto out_fput;
	}
	path->reada = 2;

	if (inode < src) {
		mutex_lock_nested(&inode->i_mutex, I_MUTEX_PARENT);
		mutex_lock_nested(&src->i_mutex, I_MUTEX_CHILD);
	} else {
		mutex_lock_nested(&src->i_mutex, I_MUTEX_PARENT);
		mutex_lock_nested(&inode->i_mutex, I_MUTEX_CHILD);
	}

	 
	ret = -EINVAL;
	if (off + len > src->i_size || off + len < off)
		goto out_unlock;
	if (len == 0)
		olen = len = src->i_size - off;
	 
	if (off + len == src->i_size)
		len = ALIGN(src->i_size, bs) - off;

	 
	if (!IS_ALIGNED(off, bs) || !IS_ALIGNED(off + len, bs) ||
	    !IS_ALIGNED(destoff, bs))
		goto out_unlock;

	if (destoff > inode->i_size) {
		ret = btrfs_cont_expand(inode, inode->i_size, destoff);
		if (ret)
			goto out_unlock;
	}

	 
	truncate_inode_pages_range(&inode->i_data, destoff,
				   PAGE_CACHE_ALIGN(destoff + len) - 1);

	 
	while (1) {
		struct btrfs_ordered_extent *ordered;
		lock_extent(&BTRFS_I(src)->io_tree, off, off + len - 1);
		ordered = btrfs_lookup_first_ordered_extent(src, off + len - 1);
		if (!ordered &&
		    !test_range_bit(&BTRFS_I(src)->io_tree, off, off + len - 1,
				    EXTENT_DELALLOC, 0, NULL))
			break;
		unlock_extent(&BTRFS_I(src)->io_tree, off, off + len - 1);
		if (ordered)
			btrfs_put_ordered_extent(ordered);
		btrfs_wait_ordered_range(src, off, len);
	}

	 
	key.objectid = btrfs_ino(src);
	key.type = BTRFS_EXTENT_DATA_KEY;
	key.offset = 0;

	while (1) {
		 
		ret = btrfs_search_slot(NULL, BTRFS_I(src)->root, &key, path,
				0, 0);
		if (ret < 0)
			goto out;

		nritems = btrfs_header_nritems(path->nodes[0]);
		if (path->slots[0] >= nritems) {
			ret = btrfs_next_leaf(BTRFS_I(src)->root, path);
			if (ret < 0)
				goto out;
			if (ret > 0)
				break;
			nritems = btrfs_header_nritems(path->nodes[0]);
		}
		leaf = path->nodes[0];
		slot = path->slots[0];

		btrfs_item_key_to_cpu(leaf, &key, slot);
		if (btrfs_key_type(&key) > BTRFS_EXTENT_DATA_KEY ||
		    key.objectid != btrfs_ino(src))
			break;

		if (btrfs_key_type(&key) == BTRFS_EXTENT_DATA_KEY) {
			struct btrfs_file_extent_item *extent;
			int type;
			u32 size;
			struct btrfs_key new_key;
			u64 disko = 0, diskl = 0;
			u64 datao = 0, datal = 0;
			u8 comp;
			u64 endoff;

			size = btrfs_item_size_nr(leaf, slot);
			read_extent_buffer(leaf, buf,
					   btrfs_item_ptr_offset(leaf, slot),
					   size);

			extent = btrfs_item_ptr(leaf, slot,
						struct btrfs_file_extent_item);
			comp = btrfs_file_extent_compression(leaf, extent);
			type = btrfs_file_extent_type(leaf, extent);
			if (type == BTRFS_FILE_EXTENT_REG ||
			    type == BTRFS_FILE_EXTENT_PREALLOC) {
				disko = btrfs_file_extent_disk_bytenr(leaf,
								      extent);
				diskl = btrfs_file_extent_disk_num_bytes(leaf,
								 extent);
				datao = btrfs_file_extent_offset(leaf, extent);
				datal = btrfs_file_extent_num_bytes(leaf,
								    extent);
			} else if (type == BTRFS_FILE_EXTENT_INLINE) {
				 
				datal = btrfs_file_extent_ram_bytes(leaf,
								    extent);
			}
			btrfs_release_path(path);

			if (key.offset + datal <= off ||
			    key.offset >= off + len - 1)
				goto next;

			memcpy(&new_key, &key, sizeof(new_key));
			new_key.objectid = btrfs_ino(inode);
			if (off <= key.offset)
				new_key.offset = key.offset + destoff - off;
			else
				new_key.offset = destoff;

			 
			trans = btrfs_start_transaction(root, 3);
			if (IS_ERR(trans)) {
				ret = PTR_ERR(trans);
				goto out;
			}

			if (type == BTRFS_FILE_EXTENT_REG ||
			    type == BTRFS_FILE_EXTENT_PREALLOC) {
				 

				 
				if (key.offset + datal > off + len)
					datal = off + len - key.offset;

				 
				if (off > key.offset) {
					datao += off - key.offset;
					datal -= off - key.offset;
				}

				ret = btrfs_drop_extents(trans, root, inode,
							 new_key.offset,
							 new_key.offset + datal,
							 1);
				if (ret) {
					btrfs_abort_transaction(trans, root,
								ret);
					btrfs_end_transaction(trans, root);
					goto out;
				}

				ret = btrfs_insert_empty_item(trans, root, path,
							      &new_key, size);
				if (ret) {
					btrfs_abort_transaction(trans, root,
								ret);
					btrfs_end_transaction(trans, root);
					goto out;
				}

				leaf = path->nodes[0];
				slot = path->slots[0];
				write_extent_buffer(leaf, buf,
					    btrfs_item_ptr_offset(leaf, slot),
					    size);

				extent = btrfs_item_ptr(leaf, slot,
						struct btrfs_file_extent_item);

				 
				if (!disko)
					datao = 0;

				btrfs_set_file_extent_offset(leaf, extent,
							     datao);
				btrfs_set_file_extent_num_bytes(leaf, extent,
								datal);
				if (disko) {
					inode_add_bytes(inode, datal);
					ret = btrfs_inc_extent_ref(trans, root,
							disko, diskl, 0,
							root->root_key.objectid,
							btrfs_ino(inode),
							new_key.offset - datao,
							0);
					if (ret) {
						btrfs_abort_transaction(trans,
									root,
									ret);
						btrfs_end_transaction(trans,
								      root);
						goto out;

					}
				}
			} else if (type == BTRFS_FILE_EXTENT_INLINE) {
				u64 skip = 0;
				u64 trim = 0;
				if (off > key.offset) {
					skip = off - key.offset;
					new_key.offset += skip;
				}

				if (key.offset + datal > off + len)
					trim = key.offset + datal - (off + len);

				if (comp && (skip || trim)) {
					ret = -EINVAL;
					btrfs_end_transaction(trans, root);
					goto out;
				}
				size -= skip + trim;
				datal -= skip + trim;

				ret = btrfs_drop_extents(trans, root, inode,
							 new_key.offset,
							 new_key.offset + datal,
							 1);
				if (ret) {
					btrfs_abort_transaction(trans, root,
								ret);
					btrfs_end_transaction(trans, root);
					goto out;
				}

				ret = btrfs_insert_empty_item(trans, root, path,
							      &new_key, size);
				if (ret) {
					btrfs_abort_transaction(trans, root,
								ret);
					btrfs_end_transaction(trans, root);
					goto out;
				}

				if (skip) {
					u32 start =
					  btrfs_file_extent_calc_inline_size(0);
					memmove(buf+start, buf+start+skip,
						datal);
				}

				leaf = path->nodes[0];
				slot = path->slots[0];
				write_extent_buffer(leaf, buf,
					    btrfs_item_ptr_offset(leaf, slot),
					    size);
				inode_add_bytes(inode, datal);
			}

			btrfs_mark_buffer_dirty(leaf);
			btrfs_release_path(path);

			inode_inc_iversion(inode);
			inode->i_mtime = inode->i_ctime = CURRENT_TIME;

			 
			endoff = new_key.offset + datal;
			if (endoff > destoff+olen)
				endoff = destoff+olen;
			if (endoff > inode->i_size)
				btrfs_i_size_write(inode, endoff);

			ret = btrfs_update_inode(trans, root, inode);
			if (ret) {
				btrfs_abort_transaction(trans, root, ret);
				btrfs_end_transaction(trans, root);
				goto out;
			}
			ret = btrfs_end_transaction(trans, root);
		}
next:
		btrfs_release_path(path);
		key.offset++;
	}
	ret = 0;
out:
	btrfs_release_path(path);
	unlock_extent(&BTRFS_I(src)->io_tree, off, off + len - 1);
out_unlock:
	mutex_unlock(&src->i_mutex);
	mutex_unlock(&inode->i_mutex);
	vfree(buf);
	btrfs_free_path(path);
out_fput:
	fdput(src_file);
out_drop_write:
	mnt_drop_write_file(file);
	return ret;
}