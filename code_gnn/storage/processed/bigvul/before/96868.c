unsigned int round_pipe_size(unsigned long size)
{
	if (size > (1U << 31))
		return 0;

	 
	if (size < PAGE_SIZE)
		return PAGE_SIZE;

	return roundup_pow_of_two(size);
}
