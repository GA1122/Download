static loff_t ext4_max_bitmap_size(int bits, int has_huge_files)
{
	loff_t res = EXT4_NDIR_BLOCKS;
	int meta_blocks;
	loff_t upper_limit;
	 

	if (!has_huge_files || sizeof(blkcnt_t) < sizeof(u64)) {
		 
		upper_limit = (1LL << 32) - 1;

		 
		upper_limit >>= (bits - 9);

	} else {
		 
		upper_limit = (1LL << 48) - 1;

	}

	 
	meta_blocks = 1;
	 
	meta_blocks += 1 + (1LL << (bits-2));
	 
	meta_blocks += 1 + (1LL << (bits-2)) + (1LL << (2*(bits-2)));

	upper_limit -= meta_blocks;
	upper_limit <<= bits;

	res += 1LL << (bits-2);
	res += 1LL << (2*(bits-2));
	res += 1LL << (3*(bits-2));
	res <<= bits;
	if (res > upper_limit)
		res = upper_limit;

	if (res > MAX_LFS_FILESIZE)
		res = MAX_LFS_FILESIZE;

	return res;
}
