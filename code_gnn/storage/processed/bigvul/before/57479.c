static int ext4_alloc_blocks(handle_t *handle, struct inode *inode,
			     ext4_lblk_t iblock, ext4_fsblk_t goal,
			     int indirect_blks, int blks,
			     ext4_fsblk_t new_blocks[4], int *err)
{
	struct ext4_allocation_request ar;
	int target, i;
	unsigned long count = 0, blk_allocated = 0;
	int index = 0;
	ext4_fsblk_t current_block = 0;
	int ret = 0;

	 
	 
	target = indirect_blks;
	while (target > 0) {
		count = target;
		 
		current_block = ext4_new_meta_blocks(handle, inode,
							goal, &count, err);
		if (*err)
			goto failed_out;

		BUG_ON(current_block + count > EXT4_MAX_BLOCK_FILE_PHYS);

		target -= count;
		 
		while (index < indirect_blks && count) {
			new_blocks[index++] = current_block++;
			count--;
		}
		if (count > 0) {
			 
			new_blocks[index] = current_block;
			printk(KERN_INFO "%s returned more blocks than "
						"requested\n", __func__);
			WARN_ON(1);
			break;
		}
	}

	target = blks - count ;
	blk_allocated = count;
	if (!target)
		goto allocated;
	 
	memset(&ar, 0, sizeof(ar));
	ar.inode = inode;
	ar.goal = goal;
	ar.len = target;
	ar.logical = iblock;
	if (S_ISREG(inode->i_mode))
		 
		ar.flags = EXT4_MB_HINT_DATA;

	current_block = ext4_mb_new_blocks(handle, &ar, err);
	BUG_ON(current_block + ar.len > EXT4_MAX_BLOCK_FILE_PHYS);

	if (*err && (target == blks)) {
		 
		goto failed_out;
	}
	if (!*err) {
		if (target == blks) {
			 
			new_blocks[index] = current_block;
		}
		blk_allocated += ar.len;
	}
allocated:
	 
	ret = blk_allocated;
	*err = 0;
	return ret;
failed_out:
	for (i = 0; i < index; i++)
		ext4_free_blocks(handle, inode, 0, new_blocks[i], 1, 0);
	return ret;
}
