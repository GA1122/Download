static noinline int copy_to_sk(struct btrfs_root *root,
			       struct btrfs_path *path,
			       struct btrfs_key *key,
			       struct btrfs_ioctl_search_key *sk,
			       char *buf,
			       unsigned long *sk_offset,
			       int *num_found)
{
	u64 found_transid;
	struct extent_buffer *leaf;
	struct btrfs_ioctl_search_header sh;
	unsigned long item_off;
	unsigned long item_len;
	int nritems;
	int i;
	int slot;
	int ret = 0;

	leaf = path->nodes[0];
	slot = path->slots[0];
	nritems = btrfs_header_nritems(leaf);

	if (btrfs_header_generation(leaf) > sk->max_transid) {
		i = nritems;
		goto advance_key;
	}
	found_transid = btrfs_header_generation(leaf);

	for (i = slot; i < nritems; i++) {
		item_off = btrfs_item_ptr_offset(leaf, i);
		item_len = btrfs_item_size_nr(leaf, i);

		if (item_len > BTRFS_SEARCH_ARGS_BUFSIZE)
			item_len = 0;

		if (sizeof(sh) + item_len + *sk_offset >
		    BTRFS_SEARCH_ARGS_BUFSIZE) {
			ret = 1;
			goto overflow;
		}

		btrfs_item_key_to_cpu(leaf, key, i);
		if (!key_in_sk(key, sk))
			continue;

		sh.objectid = key->objectid;
		sh.offset = key->offset;
		sh.type = key->type;
		sh.len = item_len;
		sh.transid = found_transid;

		 
		memcpy(buf + *sk_offset, &sh, sizeof(sh));
		*sk_offset += sizeof(sh);

		if (item_len) {
			char *p = buf + *sk_offset;
			 
			read_extent_buffer(leaf, p,
					   item_off, item_len);
			*sk_offset += item_len;
		}
		(*num_found)++;

		if (*num_found >= sk->nr_items)
			break;
	}
advance_key:
	ret = 0;
	if (key->offset < (u64)-1 && key->offset < sk->max_offset)
		key->offset++;
	else if (key->type < (u8)-1 && key->type < sk->max_type) {
		key->offset = 0;
		key->type++;
	} else if (key->objectid < (u64)-1 && key->objectid < sk->max_objectid) {
		key->offset = 0;
		key->type = 0;
		key->objectid++;
	} else
		ret = 1;
overflow:
	return ret;
}