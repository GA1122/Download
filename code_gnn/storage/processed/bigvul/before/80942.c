static inline void evmcs_write64(unsigned long field, u64 value)
{
	u16 clean_field;
	int offset = get_evmcs_offset(field, &clean_field);

	if (offset < 0)
		return;

	*(u64 *)((char *)current_evmcs + offset) = value;

	current_evmcs->hv_clean_fields &= ~clean_field;
}
