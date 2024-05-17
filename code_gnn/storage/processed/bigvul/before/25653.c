static u8 perf_event_get_msk(unsigned long val)
{
	return val & 0xff;
}
