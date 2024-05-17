static int __init boot_alloc_snapshot(char *str)
{
	allocate_snapshot = true;
	 
	ring_buffer_expanded = true;
	return 1;
}
