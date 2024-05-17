static int __btrfs_submit_bio_start_direct_io(struct inode *inode, int rw,
				    struct bio *bio, int mirror_num,
				    unsigned long bio_flags, u64 offset)
{
	int ret;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	ret = btrfs_csum_one_bio(root, inode, bio, offset, 1);
	BUG_ON(ret);  
	return 0;
}