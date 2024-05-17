static int netsnmp_session_set_security(struct snmp_session *session, char *sec_level, char *auth_protocol, char *auth_passphrase, char *priv_protocol, char *priv_passphrase, char *contextName, char *contextEngineID TSRMLS_DC)
{

	 
	if (netsnmp_session_set_sec_level(session, sec_level)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid security level '%s'", sec_level);
		return (-1);
	}

	if (session->securityLevel == SNMP_SEC_LEVEL_AUTHNOPRIV || session->securityLevel == SNMP_SEC_LEVEL_AUTHPRIV) {

		 
		if (netsnmp_session_set_auth_protocol(session, auth_protocol TSRMLS_CC)) {
			 
			return (-1);
		}

		 
		if (netsnmp_session_gen_auth_key(session, auth_passphrase TSRMLS_CC)) {
			 
			return (-1);
		}

		if (session->securityLevel == SNMP_SEC_LEVEL_AUTHPRIV) {
			 
			if (netsnmp_session_set_sec_protocol(session, priv_protocol TSRMLS_CC)) {
				 
				return (-1);
			}

			 
			if (netsnmp_session_gen_sec_key(session, priv_passphrase TSRMLS_CC)) {
				 
				return (-1);
			}
		}
	}

	 
	if (contextName) {
		session->contextName = contextName;
		session->contextNameLen = strlen(contextName);
	}

	 
	if (contextEngineID && strlen(contextEngineID) && netsnmp_session_set_contextEngineID(session, contextEngineID TSRMLS_CC)) {
		 
		return (-1);
	}

	return (0);
}
