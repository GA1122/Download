handle_mic(OM_uint32 *minor_status, gss_buffer_t mic_in,
	   int send_mechtok, spnego_gss_ctx_id_t sc,
	   gss_buffer_t *mic_out,
	   OM_uint32 *negState, send_token_flag *tokflag)
{
	OM_uint32 ret;

	ret = GSS_S_FAILURE;
	*mic_out = GSS_C_NO_BUFFER;
	if (mic_in != GSS_C_NO_BUFFER) {
		if (sc->mic_rcvd) {
			 
			*negState = REJECT;
			*tokflag = ERROR_TOKEN_SEND;
			return GSS_S_DEFECTIVE_TOKEN;
		}
	} else if (sc->mic_reqd && !send_mechtok) {
		 
		*negState = REJECT;
		*tokflag = ERROR_TOKEN_SEND;
		return GSS_S_DEFECTIVE_TOKEN;
	}
	ret = process_mic(minor_status, mic_in, sc, mic_out,
			  negState, tokflag);
	if (ret != GSS_S_COMPLETE) {
		return ret;
	}
	if (sc->mic_reqd) {
		assert(sc->mic_sent || sc->mic_rcvd);
	}
	if (sc->mic_sent && sc->mic_rcvd) {
		ret = GSS_S_COMPLETE;
		*negState = ACCEPT_COMPLETE;
		if (*mic_out == GSS_C_NO_BUFFER) {
			 
			assert(!send_mechtok);
			*tokflag = NO_TOKEN_SEND;
		} else {
			*tokflag = CONT_TOKEN_SEND;
		}
	} else if (sc->mic_reqd) {
		*negState = ACCEPT_INCOMPLETE;
		ret = GSS_S_CONTINUE_NEEDED;
	} else if (*negState == ACCEPT_COMPLETE) {
		ret = GSS_S_COMPLETE;
	} else {
		ret = GSS_S_CONTINUE_NEEDED;
	}
	return ret;
}
