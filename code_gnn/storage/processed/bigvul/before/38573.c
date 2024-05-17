static int find_highest_prio_tid(unsigned long tids)
{
	 
	if (tids & 0xF8)
		return fls(tids) - 1;
	 
	if (tids & BIT(0))
		return 0;
	return fls(tids) - 1;
}
