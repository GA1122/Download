static int ext3_fill_super (struct super_block *sb, void *data, int silent)
{
	struct buffer_head * bh;
	struct ext3_super_block *es = NULL;
	struct ext3_sb_info *sbi;
	ext3_fsblk_t block;
	ext3_fsblk_t sb_block = get_sb_block(&data, sb);
	ext3_fsblk_t logic_sb_block;
	unsigned long offset = 0;
	unsigned int journal_inum = 0;
	unsigned long journal_devnum = 0;
	unsigned long def_mount_opts;
	struct inode *root;
	int blocksize;
	int hblock;
	int db_count;
	int i;
	int needs_recovery;
	int ret = -EINVAL;
	__le32 features;
	int err;

	sbi = kzalloc(sizeof(*sbi), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;

	sbi->s_blockgroup_lock =
		kzalloc(sizeof(struct blockgroup_lock), GFP_KERNEL);
	if (!sbi->s_blockgroup_lock) {
		kfree(sbi);
		return -ENOMEM;
	}
	sb->s_fs_info = sbi;
	sbi->s_sb_block = sb_block;

	blocksize = sb_min_blocksize(sb, EXT3_MIN_BLOCK_SIZE);
	if (!blocksize) {
		ext3_msg(sb, KERN_ERR, "error: unable to set blocksize");
		goto out_fail;
	}

	 
	if (blocksize != EXT3_MIN_BLOCK_SIZE) {
		logic_sb_block = (sb_block * EXT3_MIN_BLOCK_SIZE) / blocksize;
		offset = (sb_block * EXT3_MIN_BLOCK_SIZE) % blocksize;
	} else {
		logic_sb_block = sb_block;
	}

	if (!(bh = sb_bread(sb, logic_sb_block))) {
		ext3_msg(sb, KERN_ERR, "error: unable to read superblock");
		goto out_fail;
	}
	 
	es = (struct ext3_super_block *) (bh->b_data + offset);
	sbi->s_es = es;
	sb->s_magic = le16_to_cpu(es->s_magic);
	if (sb->s_magic != EXT3_SUPER_MAGIC)
		goto cantfind_ext3;

	 
	def_mount_opts = le32_to_cpu(es->s_default_mount_opts);
	if (def_mount_opts & EXT3_DEFM_DEBUG)
		set_opt(sbi->s_mount_opt, DEBUG);
	if (def_mount_opts & EXT3_DEFM_BSDGROUPS)
		set_opt(sbi->s_mount_opt, GRPID);
	if (def_mount_opts & EXT3_DEFM_UID16)
		set_opt(sbi->s_mount_opt, NO_UID32);
#ifdef CONFIG_EXT3_FS_XATTR
	if (def_mount_opts & EXT3_DEFM_XATTR_USER)
		set_opt(sbi->s_mount_opt, XATTR_USER);
#endif
#ifdef CONFIG_EXT3_FS_POSIX_ACL
	if (def_mount_opts & EXT3_DEFM_ACL)
		set_opt(sbi->s_mount_opt, POSIX_ACL);
#endif
	if ((def_mount_opts & EXT3_DEFM_JMODE) == EXT3_DEFM_JMODE_DATA)
		set_opt(sbi->s_mount_opt, JOURNAL_DATA);
	else if ((def_mount_opts & EXT3_DEFM_JMODE) == EXT3_DEFM_JMODE_ORDERED)
		set_opt(sbi->s_mount_opt, ORDERED_DATA);
	else if ((def_mount_opts & EXT3_DEFM_JMODE) == EXT3_DEFM_JMODE_WBACK)
		set_opt(sbi->s_mount_opt, WRITEBACK_DATA);

	if (le16_to_cpu(sbi->s_es->s_errors) == EXT3_ERRORS_PANIC)
		set_opt(sbi->s_mount_opt, ERRORS_PANIC);
	else if (le16_to_cpu(sbi->s_es->s_errors) == EXT3_ERRORS_CONTINUE)
		set_opt(sbi->s_mount_opt, ERRORS_CONT);
	else
		set_opt(sbi->s_mount_opt, ERRORS_RO);

	sbi->s_resuid = make_kuid(&init_user_ns, le16_to_cpu(es->s_def_resuid));
	sbi->s_resgid = make_kgid(&init_user_ns, le16_to_cpu(es->s_def_resgid));

	 
	set_opt(sbi->s_mount_opt, BARRIER);
	set_opt(sbi->s_mount_opt, RESERVATION);

	if (!parse_options ((char *) data, sb, &journal_inum, &journal_devnum,
			    NULL, 0))
		goto failed_mount;

	sb->s_flags = (sb->s_flags & ~MS_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? MS_POSIXACL : 0);

	if (le32_to_cpu(es->s_rev_level) == EXT3_GOOD_OLD_REV &&
	    (EXT3_HAS_COMPAT_FEATURE(sb, ~0U) ||
	     EXT3_HAS_RO_COMPAT_FEATURE(sb, ~0U) ||
	     EXT3_HAS_INCOMPAT_FEATURE(sb, ~0U)))
		ext3_msg(sb, KERN_WARNING,
			"warning: feature flags set on rev 0 fs, "
			"running e2fsck is recommended");
	 
	features = EXT3_HAS_INCOMPAT_FEATURE(sb, ~EXT3_FEATURE_INCOMPAT_SUPP);
	if (features) {
		ext3_msg(sb, KERN_ERR,
			"error: couldn't mount because of unsupported "
			"optional features (%x)", le32_to_cpu(features));
		goto failed_mount;
	}
	features = EXT3_HAS_RO_COMPAT_FEATURE(sb, ~EXT3_FEATURE_RO_COMPAT_SUPP);
	if (!(sb->s_flags & MS_RDONLY) && features) {
		ext3_msg(sb, KERN_ERR,
			"error: couldn't mount RDWR because of unsupported "
			"optional features (%x)", le32_to_cpu(features));
		goto failed_mount;
	}
	blocksize = BLOCK_SIZE << le32_to_cpu(es->s_log_block_size);

	if (blocksize < EXT3_MIN_BLOCK_SIZE ||
	    blocksize > EXT3_MAX_BLOCK_SIZE) {
		ext3_msg(sb, KERN_ERR,
			"error: couldn't mount because of unsupported "
			"filesystem blocksize %d", blocksize);
		goto failed_mount;
	}

	hblock = bdev_logical_block_size(sb->s_bdev);
	if (sb->s_blocksize != blocksize) {
		 
		if (blocksize < hblock) {
			ext3_msg(sb, KERN_ERR,
				"error: fsblocksize %d too small for "
				"hardware sectorsize %d", blocksize, hblock);
			goto failed_mount;
		}

		brelse (bh);
		if (!sb_set_blocksize(sb, blocksize)) {
			ext3_msg(sb, KERN_ERR,
				"error: bad blocksize %d", blocksize);
			goto out_fail;
		}
		logic_sb_block = (sb_block * EXT3_MIN_BLOCK_SIZE) / blocksize;
		offset = (sb_block * EXT3_MIN_BLOCK_SIZE) % blocksize;
		bh = sb_bread(sb, logic_sb_block);
		if (!bh) {
			ext3_msg(sb, KERN_ERR,
			       "error: can't read superblock on 2nd try");
			goto failed_mount;
		}
		es = (struct ext3_super_block *)(bh->b_data + offset);
		sbi->s_es = es;
		if (es->s_magic != cpu_to_le16(EXT3_SUPER_MAGIC)) {
			ext3_msg(sb, KERN_ERR,
				"error: magic mismatch");
			goto failed_mount;
		}
	}

	sb->s_maxbytes = ext3_max_size(sb->s_blocksize_bits);

	if (le32_to_cpu(es->s_rev_level) == EXT3_GOOD_OLD_REV) {
		sbi->s_inode_size = EXT3_GOOD_OLD_INODE_SIZE;
		sbi->s_first_ino = EXT3_GOOD_OLD_FIRST_INO;
	} else {
		sbi->s_inode_size = le16_to_cpu(es->s_inode_size);
		sbi->s_first_ino = le32_to_cpu(es->s_first_ino);
		if ((sbi->s_inode_size < EXT3_GOOD_OLD_INODE_SIZE) ||
		    (!is_power_of_2(sbi->s_inode_size)) ||
		    (sbi->s_inode_size > blocksize)) {
			ext3_msg(sb, KERN_ERR,
				"error: unsupported inode size: %d",
				sbi->s_inode_size);
			goto failed_mount;
		}
	}
	sbi->s_frag_size = EXT3_MIN_FRAG_SIZE <<
				   le32_to_cpu(es->s_log_frag_size);
	if (blocksize != sbi->s_frag_size) {
		ext3_msg(sb, KERN_ERR,
		       "error: fragsize %lu != blocksize %u (unsupported)",
		       sbi->s_frag_size, blocksize);
		goto failed_mount;
	}
	sbi->s_frags_per_block = 1;
	sbi->s_blocks_per_group = le32_to_cpu(es->s_blocks_per_group);
	sbi->s_frags_per_group = le32_to_cpu(es->s_frags_per_group);
	sbi->s_inodes_per_group = le32_to_cpu(es->s_inodes_per_group);
	if (EXT3_INODE_SIZE(sb) == 0 || EXT3_INODES_PER_GROUP(sb) == 0)
		goto cantfind_ext3;
	sbi->s_inodes_per_block = blocksize / EXT3_INODE_SIZE(sb);
	if (sbi->s_inodes_per_block == 0)
		goto cantfind_ext3;
	sbi->s_itb_per_group = sbi->s_inodes_per_group /
					sbi->s_inodes_per_block;
	sbi->s_desc_per_block = blocksize / sizeof(struct ext3_group_desc);
	sbi->s_sbh = bh;
	sbi->s_mount_state = le16_to_cpu(es->s_state);
	sbi->s_addr_per_block_bits = ilog2(EXT3_ADDR_PER_BLOCK(sb));
	sbi->s_desc_per_block_bits = ilog2(EXT3_DESC_PER_BLOCK(sb));
	for (i=0; i < 4; i++)
		sbi->s_hash_seed[i] = le32_to_cpu(es->s_hash_seed[i]);
	sbi->s_def_hash_version = es->s_def_hash_version;
	i = le32_to_cpu(es->s_flags);
	if (i & EXT2_FLAGS_UNSIGNED_HASH)
		sbi->s_hash_unsigned = 3;
	else if ((i & EXT2_FLAGS_SIGNED_HASH) == 0) {
#ifdef __CHAR_UNSIGNED__
		es->s_flags |= cpu_to_le32(EXT2_FLAGS_UNSIGNED_HASH);
		sbi->s_hash_unsigned = 3;
#else
		es->s_flags |= cpu_to_le32(EXT2_FLAGS_SIGNED_HASH);
#endif
	}

	if (sbi->s_blocks_per_group > blocksize * 8) {
		ext3_msg(sb, KERN_ERR,
			"#blocks per group too big: %lu",
			sbi->s_blocks_per_group);
		goto failed_mount;
	}
	if (sbi->s_frags_per_group > blocksize * 8) {
		ext3_msg(sb, KERN_ERR,
			"error: #fragments per group too big: %lu",
			sbi->s_frags_per_group);
		goto failed_mount;
	}
	if (sbi->s_inodes_per_group > blocksize * 8) {
		ext3_msg(sb, KERN_ERR,
			"error: #inodes per group too big: %lu",
			sbi->s_inodes_per_group);
		goto failed_mount;
	}

	err = generic_check_addressable(sb->s_blocksize_bits,
					le32_to_cpu(es->s_blocks_count));
	if (err) {
		ext3_msg(sb, KERN_ERR,
			"error: filesystem is too large to mount safely");
		if (sizeof(sector_t) < 8)
			ext3_msg(sb, KERN_ERR,
				"error: CONFIG_LBDAF not enabled");
		ret = err;
		goto failed_mount;
	}

	if (EXT3_BLOCKS_PER_GROUP(sb) == 0)
		goto cantfind_ext3;
	sbi->s_groups_count = ((le32_to_cpu(es->s_blocks_count) -
			       le32_to_cpu(es->s_first_data_block) - 1)
				       / EXT3_BLOCKS_PER_GROUP(sb)) + 1;
	db_count = DIV_ROUND_UP(sbi->s_groups_count, EXT3_DESC_PER_BLOCK(sb));
	sbi->s_group_desc = kmalloc(db_count * sizeof (struct buffer_head *),
				    GFP_KERNEL);
	if (sbi->s_group_desc == NULL) {
		ext3_msg(sb, KERN_ERR,
			"error: not enough memory");
		ret = -ENOMEM;
		goto failed_mount;
	}

	bgl_lock_init(sbi->s_blockgroup_lock);

	for (i = 0; i < db_count; i++) {
		block = descriptor_loc(sb, logic_sb_block, i);
		sbi->s_group_desc[i] = sb_bread(sb, block);
		if (!sbi->s_group_desc[i]) {
			ext3_msg(sb, KERN_ERR,
				"error: can't read group descriptor %d", i);
			db_count = i;
			goto failed_mount2;
		}
	}
	if (!ext3_check_descriptors (sb)) {
		ext3_msg(sb, KERN_ERR,
			"error: group descriptors corrupted");
		goto failed_mount2;
	}
	sbi->s_gdb_count = db_count;
	get_random_bytes(&sbi->s_next_generation, sizeof(u32));
	spin_lock_init(&sbi->s_next_gen_lock);

	 
	spin_lock_init(&sbi->s_rsv_window_lock);
	sbi->s_rsv_window_root = RB_ROOT;
	 
	sbi->s_rsv_window_head.rsv_start = EXT3_RESERVE_WINDOW_NOT_ALLOCATED;
	sbi->s_rsv_window_head.rsv_end = EXT3_RESERVE_WINDOW_NOT_ALLOCATED;
	sbi->s_rsv_window_head.rsv_alloc_hit = 0;
	sbi->s_rsv_window_head.rsv_goal_size = 0;
	ext3_rsv_window_add(sb, &sbi->s_rsv_window_head);

	 
	sb->s_op = &ext3_sops;
	sb->s_export_op = &ext3_export_ops;
	sb->s_xattr = ext3_xattr_handlers;
#ifdef CONFIG_QUOTA
	sb->s_qcop = &ext3_qctl_operations;
	sb->dq_op = &ext3_quota_operations;
#endif
	memcpy(sb->s_uuid, es->s_uuid, sizeof(es->s_uuid));
	INIT_LIST_HEAD(&sbi->s_orphan);  
	mutex_init(&sbi->s_orphan_lock);
	mutex_init(&sbi->s_resize_lock);

	sb->s_root = NULL;

	needs_recovery = (es->s_last_orphan != 0 ||
			  EXT3_HAS_INCOMPAT_FEATURE(sb,
				    EXT3_FEATURE_INCOMPAT_RECOVER));

	 
	if (!test_opt(sb, NOLOAD) &&
	    EXT3_HAS_COMPAT_FEATURE(sb, EXT3_FEATURE_COMPAT_HAS_JOURNAL)) {
		if (ext3_load_journal(sb, es, journal_devnum))
			goto failed_mount2;
	} else if (journal_inum) {
		if (ext3_create_journal(sb, es, journal_inum))
			goto failed_mount2;
	} else {
		if (!silent)
			ext3_msg(sb, KERN_ERR,
				"error: no journal found. "
				"mounting ext3 over ext2?");
		goto failed_mount2;
	}
	err = percpu_counter_init(&sbi->s_freeblocks_counter,
			ext3_count_free_blocks(sb));
	if (!err) {
		err = percpu_counter_init(&sbi->s_freeinodes_counter,
				ext3_count_free_inodes(sb));
	}
	if (!err) {
		err = percpu_counter_init(&sbi->s_dirs_counter,
				ext3_count_dirs(sb));
	}
	if (err) {
		ext3_msg(sb, KERN_ERR, "error: insufficient memory");
		ret = err;
		goto failed_mount3;
	}

	 
	switch (test_opt(sb, DATA_FLAGS)) {
	case 0:
		 
		if (journal_check_available_features
		    (sbi->s_journal, 0, 0, JFS_FEATURE_INCOMPAT_REVOKE))
			set_opt(sbi->s_mount_opt, DEFAULT_DATA_MODE);
		else
			set_opt(sbi->s_mount_opt, JOURNAL_DATA);
		break;

	case EXT3_MOUNT_ORDERED_DATA:
	case EXT3_MOUNT_WRITEBACK_DATA:
		if (!journal_check_available_features
		    (sbi->s_journal, 0, 0, JFS_FEATURE_INCOMPAT_REVOKE)) {
			ext3_msg(sb, KERN_ERR,
				"error: journal does not support "
				"requested data journaling mode");
			goto failed_mount3;
		}
	default:
		break;
	}

	 

	root = ext3_iget(sb, EXT3_ROOT_INO);
	if (IS_ERR(root)) {
		ext3_msg(sb, KERN_ERR, "error: get root inode failed");
		ret = PTR_ERR(root);
		goto failed_mount3;
	}
	if (!S_ISDIR(root->i_mode) || !root->i_blocks || !root->i_size) {
		iput(root);
		ext3_msg(sb, KERN_ERR, "error: corrupt root inode, run e2fsck");
		goto failed_mount3;
	}
	sb->s_root = d_make_root(root);
	if (!sb->s_root) {
		ext3_msg(sb, KERN_ERR, "error: get root dentry failed");
		ret = -ENOMEM;
		goto failed_mount3;
	}

	if (ext3_setup_super(sb, es, sb->s_flags & MS_RDONLY))
		sb->s_flags |= MS_RDONLY;

	EXT3_SB(sb)->s_mount_state |= EXT3_ORPHAN_FS;
	ext3_orphan_cleanup(sb, es);
	EXT3_SB(sb)->s_mount_state &= ~EXT3_ORPHAN_FS;
	if (needs_recovery) {
		ext3_mark_recovery_complete(sb, es);
		ext3_msg(sb, KERN_INFO, "recovery complete");
	}
	ext3_msg(sb, KERN_INFO, "mounted filesystem with %s data mode",
		test_opt(sb,DATA_FLAGS) == EXT3_MOUNT_JOURNAL_DATA ? "journal":
		test_opt(sb,DATA_FLAGS) == EXT3_MOUNT_ORDERED_DATA ? "ordered":
		"writeback");
	sb->s_flags |= MS_SNAP_STABLE;

	return 0;

cantfind_ext3:
	if (!silent)
		ext3_msg(sb, KERN_INFO,
			"error: can't find ext3 filesystem on dev %s.",
		       sb->s_id);
	goto failed_mount;

failed_mount3:
	percpu_counter_destroy(&sbi->s_freeblocks_counter);
	percpu_counter_destroy(&sbi->s_freeinodes_counter);
	percpu_counter_destroy(&sbi->s_dirs_counter);
	journal_destroy(sbi->s_journal);
failed_mount2:
	for (i = 0; i < db_count; i++)
		brelse(sbi->s_group_desc[i]);
	kfree(sbi->s_group_desc);
failed_mount:
#ifdef CONFIG_QUOTA
	for (i = 0; i < MAXQUOTAS; i++)
		kfree(sbi->s_qf_names[i]);
#endif
	ext3_blkdev_remove(sbi);
	brelse(bh);
out_fail:
	sb->s_fs_info = NULL;
	kfree(sbi->s_blockgroup_lock);
	kfree(sbi);
	return ret;
}