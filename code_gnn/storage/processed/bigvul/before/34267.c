static int __btrfs_submit_bio_start(struct inode *inode, int rw,
				    struct bio *bio, int mirror_num,
				    unsigned long bio_flags,
				    u64 bio_offset)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	int ret = 0;

	ret = btrfs_csum_one_bio(root, inode, bio, 0, 0);
	BUG_ON(ret);  
	return 0;
}