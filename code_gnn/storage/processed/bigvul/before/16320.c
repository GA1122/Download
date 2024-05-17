void BaseShadow::config()
{
	if (spool) free(spool);
	spool = param("SPOOL");
	if (!spool) {
		EXCEPT("SPOOL not specified in config file.");
	}

	if (fsDomain) free(fsDomain);
	fsDomain = param( "FILESYSTEM_DOMAIN" );
	if (!fsDomain) {
		EXCEPT("FILESYSTEM_DOMAIN not specified in config file.");
	}

	if (uidDomain) free(uidDomain);
	uidDomain = param( "UID_DOMAIN" );
	if (!uidDomain) {
		EXCEPT("UID_DOMAIN not specified in config file.");
	}

	reconnect_ceiling = param_integer( "RECONNECT_BACKOFF_CEILING", 300 );

	reconnect_e_factor = 0.0;
	reconnect_e_factor = param_double( "RECONNECT_BACKOFF_FACTOR", 2.0, 0.0 );
	if( reconnect_e_factor < -1e-4 || reconnect_e_factor > 1e-4) {
    	reconnect_e_factor = 2.0;
    }

	m_cleanup_retry_tid = -1;
	m_num_cleanup_retries = 0;
	m_max_cleanup_retries = param_integer("SHADOW_MAX_JOB_CLEANUP_RETRIES", 5);
	m_cleanup_retry_delay = param_integer("SHADOW_JOB_CLEANUP_RETRY_DELAY", 30);

	m_lazy_queue_update = param_boolean("SHADOW_LAZY_QUEUE_UPDATE", true);
}
