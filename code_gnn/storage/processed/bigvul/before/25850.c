static bool p4_event_match_cpu_model(unsigned int event_idx)
{
	 
	if (event_idx == P4_EVENT_INSTR_COMPLETED) {
		if (boot_cpu_data.x86_model != 3 &&
			boot_cpu_data.x86_model != 4 &&
			boot_cpu_data.x86_model != 6)
			return false;
	}

	 

	return true;
}
