static size_t taskstats_packet_size(void)
{
	size_t size;

	size = nla_total_size(sizeof(u32)) +
		nla_total_size(sizeof(struct taskstats)) + nla_total_size(0);
#ifdef TASKSTATS_NEEDS_PADDING
	size += nla_total_size(0);  
#endif
	return size;
}