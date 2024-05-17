static void add_msr_offset(u32 offset)
{
	int i;

	for (i = 0; i < MSRPM_OFFSETS; ++i) {

		 
		if (msrpm_offsets[i] == offset)
			return;

		 
		if (msrpm_offsets[i] != MSR_INVALID)
			continue;

		 
		msrpm_offsets[i] = offset;

		return;
	}

	 
	BUG();
}
