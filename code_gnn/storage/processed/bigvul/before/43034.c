static void jit_fill_hole(void *area, unsigned int size)
{
	 
	memset(area, 0xcc, size);
}
