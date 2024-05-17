static int __f2fs_issue_discard_zone(struct f2fs_sb_info *sbi,
		struct block_device *bdev, block_t blkstart, block_t blklen)
{
	sector_t sector, nr_sects;
	block_t lblkstart = blkstart;
	int devi = 0;

	if (sbi->s_ndevs) {
		devi = f2fs_target_device_index(sbi, blkstart);
		blkstart -= FDEV(devi).start_blk;
	}

	 
	switch (get_blkz_type(sbi, bdev, blkstart)) {

	case BLK_ZONE_TYPE_CONVENTIONAL:
		if (!blk_queue_discard(bdev_get_queue(bdev)))
			return 0;
		return __queue_discard_cmd(sbi, bdev, lblkstart, blklen);
	case BLK_ZONE_TYPE_SEQWRITE_REQ:
	case BLK_ZONE_TYPE_SEQWRITE_PREF:
		sector = SECTOR_FROM_BLOCK(blkstart);
		nr_sects = SECTOR_FROM_BLOCK(blklen);

		if (sector & (bdev_zone_sectors(bdev) - 1) ||
				nr_sects != bdev_zone_sectors(bdev)) {
			f2fs_msg(sbi->sb, KERN_INFO,
				"(%d) %s: Unaligned discard attempted (block %x + %x)",
				devi, sbi->s_ndevs ? FDEV(devi).path: "",
				blkstart, blklen);
			return -EIO;
		}
		trace_f2fs_issue_reset_zone(bdev, blkstart);
		return blkdev_reset_zones(bdev, sector,
					  nr_sects, GFP_NOFS);
	default:
		 
		return -EIO;
	}
}
