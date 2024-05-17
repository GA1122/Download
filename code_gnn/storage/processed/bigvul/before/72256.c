process_gssapi_token(void *ctxt, gss_buffer_t recv_tok)
{
	Authctxt *authctxt = ctxt;
	Gssctxt *gssctxt = authctxt->methoddata;
	gss_buffer_desc send_tok = GSS_C_EMPTY_BUFFER;
	gss_buffer_desc mic = GSS_C_EMPTY_BUFFER;
	gss_buffer_desc gssbuf;
	OM_uint32 status, ms, flags;
	Buffer b;

	status = ssh_gssapi_init_ctx(gssctxt, options.gss_deleg_creds,
	    recv_tok, &send_tok, &flags);

	if (send_tok.length > 0) {
		if (GSS_ERROR(status))
			packet_start(SSH2_MSG_USERAUTH_GSSAPI_ERRTOK);
		else
			packet_start(SSH2_MSG_USERAUTH_GSSAPI_TOKEN);

		packet_put_string(send_tok.value, send_tok.length);
		packet_send();
		gss_release_buffer(&ms, &send_tok);
	}

	if (status == GSS_S_COMPLETE) {
		 
		if (!(flags & GSS_C_INTEG_FLAG)) {
			packet_start(SSH2_MSG_USERAUTH_GSSAPI_EXCHANGE_COMPLETE);
			packet_send();
		} else {
			ssh_gssapi_buildmic(&b, authctxt->server_user,
			    authctxt->service, "gssapi-with-mic");

			gssbuf.value = buffer_ptr(&b);
			gssbuf.length = buffer_len(&b);

			status = ssh_gssapi_sign(gssctxt, &gssbuf, &mic);

			if (!GSS_ERROR(status)) {
				packet_start(SSH2_MSG_USERAUTH_GSSAPI_MIC);
				packet_put_string(mic.value, mic.length);

				packet_send();
			}

			buffer_free(&b);
			gss_release_buffer(&ms, &mic);
		}
	}

	return status;
}
