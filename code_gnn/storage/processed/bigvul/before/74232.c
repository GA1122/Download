uninit_util(void)
{
#if defined(_MSC_VER) && defined (_DEBUG)
	_CrtCheckMemory();
#endif
	if (stats_drift_file) {
		free(stats_drift_file);
		free(stats_temp_file);
		stats_drift_file = NULL;
		stats_temp_file = NULL;
	}
	if (key_file_name) {
		free(key_file_name);
		key_file_name = NULL;
	}
	filegen_unregister("peerstats");
	filegen_unregister("loopstats");
	filegen_unregister("clockstats");
	filegen_unregister("rawstats");
	filegen_unregister("sysstats");
	filegen_unregister("protostats");
#ifdef AUTOKEY
	filegen_unregister("cryptostats");
#endif	 
#ifdef DEBUG_TIMING
	filegen_unregister("timingstats");
#endif	 

#if defined(_MSC_VER) && defined (_DEBUG)
	_CrtCheckMemory();
#endif
}
