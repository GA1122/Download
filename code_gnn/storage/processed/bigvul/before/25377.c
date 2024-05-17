mipspmu_map_general_event(int idx)
{
	const struct mips_perf_event *pev;

	pev = ((*mipspmu->general_event_map)[idx].event_id ==
		UNSUPPORTED_PERF_EVENT_ID ? ERR_PTR(-EOPNOTSUPP) :
		&(*mipspmu->general_event_map)[idx]);

	return pev;
}
