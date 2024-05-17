static size_t ecryptfs_max_decoded_size(size_t encoded_size)
{
	 
	return ((encoded_size + 1) * 3) / 4;
}
