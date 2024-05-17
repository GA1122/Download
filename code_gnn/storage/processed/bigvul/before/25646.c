static u64 nop_for_index(int idx)
{
	return event_encoding(idx == PIC_UPPER_INDEX ?
			      sparc_pmu->upper_nop :
			      sparc_pmu->lower_nop, idx);
}
