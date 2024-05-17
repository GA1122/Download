int ext4_expand_extra_isize_ea(struct inode *inode, int new_extra_isize,
			       struct ext4_inode *raw_inode, handle_t *handle)
{
	struct ext4_xattr_ibody_header *header;
	struct ext4_xattr_entry *entry, *last, *first;
	struct buffer_head *bh = NULL;
	struct ext4_xattr_ibody_find *is = NULL;
	struct ext4_xattr_block_find *bs = NULL;
	char *buffer = NULL, *b_entry_name = NULL;
	size_t min_offs, free;
	int total_ino;
	void *base, *start, *end;
	int extra_isize = 0, error = 0, tried_min_extra_isize = 0;
	int s_min_extra_isize = le16_to_cpu(EXT4_SB(inode->i_sb)->s_es->s_min_extra_isize);

	down_write(&EXT4_I(inode)->xattr_sem);
retry:
	if (EXT4_I(inode)->i_extra_isize >= new_extra_isize) {
		up_write(&EXT4_I(inode)->xattr_sem);
		return 0;
	}

	header = IHDR(inode, raw_inode);
	entry = IFIRST(header);

	 

	base = start = entry;
	end = (void *)raw_inode + EXT4_SB(inode->i_sb)->s_inode_size;
	min_offs = end - base;
	last = entry;
	total_ino = sizeof(struct ext4_xattr_ibody_header);

	free = ext4_xattr_free_space(last, &min_offs, base, &total_ino);
	if (free >= new_extra_isize) {
		entry = IFIRST(header);
		ext4_xattr_shift_entries(entry,	EXT4_I(inode)->i_extra_isize
				- new_extra_isize, (void *)raw_inode +
				EXT4_GOOD_OLD_INODE_SIZE + new_extra_isize,
				(void *)header, total_ino,
				inode->i_sb->s_blocksize);
		EXT4_I(inode)->i_extra_isize = new_extra_isize;
		error = 0;
		goto cleanup;
	}

	 
	if (EXT4_I(inode)->i_file_acl) {
		bh = sb_bread(inode->i_sb, EXT4_I(inode)->i_file_acl);
		error = -EIO;
		if (!bh)
			goto cleanup;
		if (ext4_xattr_check_block(inode, bh)) {
			EXT4_ERROR_INODE(inode, "bad block %llu",
					 EXT4_I(inode)->i_file_acl);
			error = -EFSCORRUPTED;
			goto cleanup;
		}
		base = BHDR(bh);
		first = BFIRST(bh);
		end = bh->b_data + bh->b_size;
		min_offs = end - base;
		free = ext4_xattr_free_space(first, &min_offs, base, NULL);
		if (free < new_extra_isize) {
			if (!tried_min_extra_isize && s_min_extra_isize) {
				tried_min_extra_isize++;
				new_extra_isize = s_min_extra_isize;
				brelse(bh);
				goto retry;
			}
			error = -1;
			goto cleanup;
		}
	} else {
		free = inode->i_sb->s_blocksize;
	}

	while (new_extra_isize > 0) {
		size_t offs, size, entry_size;
		struct ext4_xattr_entry *small_entry = NULL;
		struct ext4_xattr_info i = {
			.value = NULL,
			.value_len = 0,
		};
		unsigned int total_size;   
		unsigned int shift_bytes;  
		unsigned int min_total_size = ~0U;

		is = kzalloc(sizeof(struct ext4_xattr_ibody_find), GFP_NOFS);
		bs = kzalloc(sizeof(struct ext4_xattr_block_find), GFP_NOFS);
		if (!is || !bs) {
			error = -ENOMEM;
			goto cleanup;
		}

		is->s.not_found = -ENODATA;
		bs->s.not_found = -ENODATA;
		is->iloc.bh = NULL;
		bs->bh = NULL;

		last = IFIRST(header);
		 
		entry = NULL;
		for (; !IS_LAST_ENTRY(last); last = EXT4_XATTR_NEXT(last)) {
			total_size =
			EXT4_XATTR_SIZE(le32_to_cpu(last->e_value_size)) +
					EXT4_XATTR_LEN(last->e_name_len);
			if (total_size <= free && total_size < min_total_size) {
				if (total_size < new_extra_isize) {
					small_entry = last;
				} else {
					entry = last;
					min_total_size = total_size;
				}
			}
		}

		if (entry == NULL) {
			if (small_entry) {
				entry = small_entry;
			} else {
				if (!tried_min_extra_isize &&
				    s_min_extra_isize) {
					tried_min_extra_isize++;
					new_extra_isize = s_min_extra_isize;
					kfree(is); is = NULL;
					kfree(bs); bs = NULL;
					brelse(bh);
					goto retry;
				}
				error = -1;
				goto cleanup;
			}
		}
		offs = le16_to_cpu(entry->e_value_offs);
		size = le32_to_cpu(entry->e_value_size);
		entry_size = EXT4_XATTR_LEN(entry->e_name_len);
		i.name_index = entry->e_name_index,
		buffer = kmalloc(EXT4_XATTR_SIZE(size), GFP_NOFS);
		b_entry_name = kmalloc(entry->e_name_len + 1, GFP_NOFS);
		if (!buffer || !b_entry_name) {
			error = -ENOMEM;
			goto cleanup;
		}
		 
		memcpy(buffer, (void *)IFIRST(header) + offs,
		       EXT4_XATTR_SIZE(size));
		memcpy(b_entry_name, entry->e_name, entry->e_name_len);
		b_entry_name[entry->e_name_len] = '\0';
		i.name = b_entry_name;

		error = ext4_get_inode_loc(inode, &is->iloc);
		if (error)
			goto cleanup;

		error = ext4_xattr_ibody_find(inode, &i, is);
		if (error)
			goto cleanup;

		 
		error = ext4_xattr_ibody_set(handle, inode, &i, is);
		if (error)
			goto cleanup;

		entry = IFIRST(header);
		if (entry_size + EXT4_XATTR_SIZE(size) >= new_extra_isize)
			shift_bytes = new_extra_isize;
		else
			shift_bytes = entry_size + size;
		 
		ext4_xattr_shift_entries(entry, EXT4_I(inode)->i_extra_isize -
			shift_bytes, (void *)raw_inode +
			EXT4_GOOD_OLD_INODE_SIZE + extra_isize + shift_bytes,
			(void *)header, total_ino - entry_size,
			inode->i_sb->s_blocksize);

		extra_isize += shift_bytes;
		new_extra_isize -= shift_bytes;
		EXT4_I(inode)->i_extra_isize = extra_isize;

		i.name = b_entry_name;
		i.value = buffer;
		i.value_len = size;
		error = ext4_xattr_block_find(inode, &i, bs);
		if (error)
			goto cleanup;

		 
		error = ext4_xattr_block_set(handle, inode, &i, bs);
		if (error)
			goto cleanup;
		kfree(b_entry_name);
		kfree(buffer);
		b_entry_name = NULL;
		buffer = NULL;
		brelse(is->iloc.bh);
		kfree(is);
		kfree(bs);
	}
	brelse(bh);
	up_write(&EXT4_I(inode)->xattr_sem);
	return 0;

cleanup:
	kfree(b_entry_name);
	kfree(buffer);
	if (is)
		brelse(is->iloc.bh);
	kfree(is);
	kfree(bs);
	brelse(bh);
	up_write(&EXT4_I(inode)->xattr_sem);
	return error;
}
