core_dump_init(void)
{
	struct rlimit orig_rlim, rlim;

	if (set_core_dump_pattern) {
		 
		update_core_dump_pattern(core_dump_pattern);
	}

	if (create_core_dump) {
		rlim.rlim_cur = RLIM_INFINITY;
		rlim.rlim_max = RLIM_INFINITY;

		if (getrlimit(RLIMIT_CORE, &orig_rlim) == -1)
			log_message(LOG_INFO, "Failed to get core file size");
		else if (setrlimit(RLIMIT_CORE, &rlim) == -1)
			log_message(LOG_INFO, "Failed to set core file size");
		else
			set_child_rlimit(RLIMIT_CORE, &orig_rlim);
	}
}