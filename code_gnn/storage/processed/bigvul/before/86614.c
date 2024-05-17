int fpm_children_init_main()  
{
	if (fpm_global_config.emergency_restart_threshold &&
		fpm_global_config.emergency_restart_interval) {

		last_faults = malloc(sizeof(time_t) * fpm_global_config.emergency_restart_threshold);

		if (!last_faults) {
			return -1;
		}

		memset(last_faults, 0, sizeof(time_t) * fpm_global_config.emergency_restart_threshold);
	}

	if (0 > fpm_cleanup_add(FPM_CLEANUP_ALL, fpm_children_cleanup, 0)) {
		return -1;
	}

	return 0;
}
 