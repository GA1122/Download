static int ext4_htree_next_block(struct inode *dir, __u32 hash,
				 struct dx_frame *frame,
				 struct dx_frame *frames,
				 __u32 *start_hash)
{
	struct dx_frame *p;
	struct buffer_head *bh;
	int err, num_frames = 0;
	__u32 bhash;

	p = frame;
	 
	while (1) {
		if (++(p->at) < p->entries + dx_get_count(p->entries))
			break;
		if (p == frames)
			return 0;
		num_frames++;
		p--;
	}

	 
	bhash = dx_get_hash(p->at);
	if (start_hash)
		*start_hash = bhash;
	if ((hash & 1) == 0) {
		if ((bhash & ~1) != hash)
			return 0;
	}
	 
	while (num_frames--) {
		if (!(bh = ext4_bread(NULL, dir, dx_get_block(p->at),
				      0, &err))) {
			if (!err) {
				ext4_error(dir->i_sb,
					   "Directory hole detected on inode %lu\n",
					   dir->i_ino);
				return -EIO;
			}
			return err;  
		}

		if (!buffer_verified(bh) &&
		    !ext4_dx_csum_verify(dir,
					 (struct ext4_dir_entry *)bh->b_data)) {
			ext4_warning(dir->i_sb, "Node failed checksum");
			return -EIO;
		}
		set_buffer_verified(bh);

		p++;
		brelse(p->bh);
		p->bh = bh;
		p->at = p->entries = ((struct dx_node *) bh->b_data)->entries;
	}
	return 1;
}
