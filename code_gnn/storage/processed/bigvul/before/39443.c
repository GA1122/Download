static int transfer_size(int ssize, int max_sector, int max_size)
{
	SUPBOUND(max_sector, fsector_t + max_size);

	 
	max_sector -= (max_sector % _floppy->sect) % ssize;

	 
	current_count_sectors = max_sector - fsector_t;

	return max_sector;
}
