static journal_t *ext3_get_dev_journal(struct super_block *sb,
				       dev_t j_dev)
{
	struct buffer_head * bh;
	journal_t *journal;
	ext3_fsblk_t start;
	ext3_fsblk_t len;
	int hblock, blocksize;
	ext3_fsblk_t sb_block;
	unsigned long offset;
	struct ext3_super_block * es;
	struct block_device *bdev;

	bdev = ext3_blkdev_get(j_dev, sb);
	if (bdev == NULL)
		return NULL;

	blocksize = sb->s_blocksize;
	hblock = bdev_logical_block_size(bdev);
	if (blocksize < hblock) {
		ext3_msg(sb, KERN_ERR,
			"error: blocksize too small for journal device");
		goto out_bdev;
	}

	sb_block = EXT3_MIN_BLOCK_SIZE / blocksize;
	offset = EXT3_MIN_BLOCK_SIZE % blocksize;
	set_blocksize(bdev, blocksize);
	if (!(bh = __bread(bdev, sb_block, blocksize))) {
		ext3_msg(sb, KERN_ERR, "error: couldn't read superblock of "
			"external journal");
		goto out_bdev;
	}

	es = (struct ext3_super_block *) (bh->b_data + offset);
	if ((le16_to_cpu(es->s_magic) != EXT3_SUPER_MAGIC) ||
	    !(le32_to_cpu(es->s_feature_incompat) &
	      EXT3_FEATURE_INCOMPAT_JOURNAL_DEV)) {
		ext3_msg(sb, KERN_ERR, "error: external journal has "
			"bad superblock");
		brelse(bh);
		goto out_bdev;
	}

	if (memcmp(EXT3_SB(sb)->s_es->s_journal_uuid, es->s_uuid, 16)) {
		ext3_msg(sb, KERN_ERR, "error: journal UUID does not match");
		brelse(bh);
		goto out_bdev;
	}

	len = le32_to_cpu(es->s_blocks_count);
	start = sb_block + 1;
	brelse(bh);	 

	journal = journal_init_dev(bdev, sb->s_bdev,
					start, len, blocksize);
	if (!journal) {
		ext3_msg(sb, KERN_ERR,
			"error: failed to create device journal");
		goto out_bdev;
	}
	journal->j_private = sb;
	if (!bh_uptodate_or_lock(journal->j_sb_buffer)) {
		if (bh_submit_read(journal->j_sb_buffer)) {
			ext3_msg(sb, KERN_ERR, "I/O error on journal device");
			goto out_journal;
		}
	}
	if (be32_to_cpu(journal->j_superblock->s_nr_users) != 1) {
		ext3_msg(sb, KERN_ERR,
			"error: external journal has more than one "
			"user (unsupported) - %d",
			be32_to_cpu(journal->j_superblock->s_nr_users));
		goto out_journal;
	}
	EXT3_SB(sb)->journal_bdev = bdev;
	ext3_init_journal_params(sb, journal);
	return journal;
out_journal:
	journal_destroy(journal);
out_bdev:
	ext3_blkdev_put(bdev);
	return NULL;
}