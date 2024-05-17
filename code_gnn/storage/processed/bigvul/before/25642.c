static u64 mask_for_index(int idx)
{
	return event_encoding(sparc_pmu->event_mask, idx);
}
