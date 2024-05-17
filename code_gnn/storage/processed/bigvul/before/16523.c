process_dynamic_configs()
{
	int per_rval = 0;
	int run_rval = 0;

	init_dynamic_config();

	if( enable_persistent ) {
		per_rval = process_persistent_configs();
	}

	if( enable_runtime ) {
		run_rval = process_runtime_configs();
	}

	if( per_rval < 0 || run_rval < 0 ) {
		return -1;
	}
	if( per_rval || run_rval ) {
		return 1;
	}
	return 0;
}
