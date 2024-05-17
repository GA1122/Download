static loff_t max_file_blocks(void)
{
	loff_t result = 0;
	loff_t leaf_count = ADDRS_PER_BLOCK;

	 

	 
	result += (leaf_count * 2);

	 
	leaf_count *= NIDS_PER_BLOCK;
	result += (leaf_count * 2);

	 
	leaf_count *= NIDS_PER_BLOCK;
	result += leaf_count;

	return result;
}
