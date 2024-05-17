set_max_delegations(void)
{
	 
	max_delegations = nr_free_buffer_pages() >> (20 - 2 - PAGE_SHIFT);
}
