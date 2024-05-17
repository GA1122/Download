unsigned long to_ratio(u64 period, u64 runtime)
{
	if (runtime == RUNTIME_INF)
		return 1ULL << 20;

	 
	if (period == 0)
		return 0;

	return div64_u64(runtime << 20, period);
}
