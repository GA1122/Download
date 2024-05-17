CStarter::Config()
{
	if( Execute ) {
		free( Execute );
	}
	if( (Execute = param("EXECUTE")) == NULL ) {
		if( is_gridshell ) {
			Execute = strdup( orig_cwd );
		} else {
			EXCEPT("Execute directory not specified in config file.");
		}
	}
	if (!m_configured) {
		bool ps = privsep_enabled();
		bool gl = param_boolean("GLEXEC_JOB", false);
#if !defined(LINUX)
		dprintf(D_ALWAYS,
		        "GLEXEC_JOB not supported on this platform; "
		            "ignoring\n");
		gl = false;
#endif
		if (ps && gl) {
			EXCEPT("can't support both "
			           "PRIVSEP_ENABLED and GLEXEC_JOB");
		}
		if (ps) {
			m_privsep_helper = new CondorPrivSepHelper;
			ASSERT(m_privsep_helper != NULL);
		}
		else if (gl) {
#if defined(LINUX)
			m_privsep_helper = new GLExecPrivSepHelper;
			ASSERT(m_privsep_helper != NULL);
#endif
		}
	}

	jic->config();

	m_configured = true;
}
