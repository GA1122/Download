static bool start_new_rx_buffer(int offset, unsigned long size, int head)
{
	 
	if (offset == MAX_BUFFER_OFFSET)
		return true;

	 
	if ((offset + size > MAX_BUFFER_OFFSET) &&
	    (size <= MAX_BUFFER_OFFSET) && offset && !head)
		return true;

	return false;
}
