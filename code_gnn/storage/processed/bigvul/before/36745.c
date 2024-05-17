spnego_gss_display_status(
		OM_uint32 *minor_status,
		OM_uint32 status_value,
		int status_type,
		gss_OID mech_type,
		OM_uint32 *message_context,
		gss_buffer_t status_string)
{
	OM_uint32 maj = GSS_S_COMPLETE;
	int ret;

	dsyslog("Entering display_status\n");

	*message_context = 0;
	switch (status_value) {
	    case ERR_SPNEGO_NO_MECHS_AVAILABLE:
		 
		*status_string = make_err_msg(_("SPNEGO cannot find "
						"mechanisms to negotiate"));
		break;
	    case ERR_SPNEGO_NO_CREDS_ACQUIRED:
		 
		*status_string = make_err_msg(_("SPNEGO failed to acquire "
						"creds"));
		break;
	    case ERR_SPNEGO_NO_MECH_FROM_ACCEPTOR:
		 
		*status_string = make_err_msg(_("SPNEGO acceptor did not "
						"select a mechanism"));
		break;
	    case ERR_SPNEGO_NEGOTIATION_FAILED:
		 
		*status_string = make_err_msg(_("SPNEGO failed to negotiate a "
						"mechanism"));
		break;
	    case ERR_SPNEGO_NO_TOKEN_FROM_ACCEPTOR:
		 
		*status_string = make_err_msg(_("SPNEGO acceptor did not "
						"return a valid token"));
		break;
	    default:
		 
		if (k5_getspecific(K5_KEY_GSS_SPNEGO_STATUS) != NULL) {
			 
			const char *err = error_message(status_value);
			*status_string = make_err_msg(err);
			break;
		}
		 
		ret = k5_setspecific(K5_KEY_GSS_SPNEGO_STATUS, &ret);
		if (ret != 0) {
			*minor_status = ret;
			maj = GSS_S_FAILURE;
			break;
		}
		maj = gss_display_status(minor_status, status_value,
					 status_type, mech_type,
					 message_context, status_string);
		 
		(void)k5_setspecific(K5_KEY_GSS_SPNEGO_STATUS, NULL);
		break;
	}

	dsyslog("Leaving display_status\n");
	return maj;
}
