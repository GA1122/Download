userauth_gssapi(Authctxt *authctxt)
{
	Gssctxt *gssctxt = NULL;
	static gss_OID_set gss_supported = NULL;
	static u_int mech = 0;
	OM_uint32 min;
	int ok = 0;

	 

	if (gss_supported == NULL)
		gss_indicate_mechs(&min, &gss_supported);

	 
	while (mech < gss_supported->count && !ok) {
		 
		if (gss_supported->elements[mech].length < 128 &&
		    ssh_gssapi_check_mechanism(&gssctxt, 
		    &gss_supported->elements[mech], authctxt->host)) {
			ok = 1;  
		} else {
			mech++;
		}
	}

	if (!ok)
		return 0;

	authctxt->methoddata=(void *)gssctxt;

	packet_start(SSH2_MSG_USERAUTH_REQUEST);
	packet_put_cstring(authctxt->server_user);
	packet_put_cstring(authctxt->service);
	packet_put_cstring(authctxt->method->name);

	packet_put_int(1);

	packet_put_int((gss_supported->elements[mech].length) + 2);
	packet_put_char(SSH_GSS_OIDTYPE);
	packet_put_char(gss_supported->elements[mech].length);
	packet_put_raw(gss_supported->elements[mech].elements,
	    gss_supported->elements[mech].length);

	packet_send();

	dispatch_set(SSH2_MSG_USERAUTH_GSSAPI_RESPONSE, &input_gssapi_response);
	dispatch_set(SSH2_MSG_USERAUTH_GSSAPI_TOKEN, &input_gssapi_token);
	dispatch_set(SSH2_MSG_USERAUTH_GSSAPI_ERROR, &input_gssapi_error);
	dispatch_set(SSH2_MSG_USERAUTH_GSSAPI_ERRTOK, &input_gssapi_errtok);

	mech++;  

	return 1;
}
