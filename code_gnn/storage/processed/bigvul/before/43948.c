static inline int valid_request(int offset, int size)
{
	 
	if ((size == 1 || size == 2 || size == 4) && (offset % size) == 0)
		return 1;
	return 0;
}
