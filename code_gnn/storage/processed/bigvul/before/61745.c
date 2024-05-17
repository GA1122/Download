static void __init tcp_init_mem(void)
{
	unsigned long limit = nr_free_buffer_pages() / 16;

	limit = max(limit, 128UL);
	sysctl_tcp_mem[0] = limit / 4 * 3;		 
	sysctl_tcp_mem[1] = limit;			 
	sysctl_tcp_mem[2] = sysctl_tcp_mem[0] * 2;	 
}
