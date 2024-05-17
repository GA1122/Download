int btrfs_merge_bio_hook(struct page *page, unsigned long offset,
			 size_t size, struct bio *bio,
			 unsigned long bio_flags)
{
	struct btrfs_root *root = BTRFS_I(page->mapping->host)->root;
	u64 logical = (u64)bio->bi_sector << 9;
	u64 length = 0;
	u64 map_length;
	int ret;

	if (bio_flags & EXTENT_BIO_COMPRESSED)
		return 0;

	length = bio->bi_size;
	map_length = length;
	ret = btrfs_map_block(root->fs_info, READ, logical,
			      &map_length, NULL, 0);
	 
	BUG_ON(ret < 0);
	if (map_length < length + size)
		return 1;
	return 0;
}