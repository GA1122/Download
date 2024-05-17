static loff_t ext2_max_size(int bits)
{
	loff_t res = EXT2_NDIR_BLOCKS;
	int meta_blocks;
	loff_t upper_limit;

	 
	upper_limit = (1LL << 32) - 1;

	 
	upper_limit >>= (bits - 9);


	 
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
