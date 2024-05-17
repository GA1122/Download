static unsigned long __init get_new_step_size(unsigned long step_size)
{
	 
	return step_size << (PMD_SHIFT - PAGE_SHIFT - 1);
}
