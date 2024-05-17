init_ctx_reselect(OM_uint32 *minor_status, spnego_gss_ctx_id_t sc,
		  OM_uint32 acc_negState, gss_OID supportedMech,
		  gss_buffer_t *responseToken, gss_buffer_t *mechListMIC,
		  OM_uint32 *negState, send_token_flag *tokflag)
{
	OM_uint32 tmpmin;
	size_t i;

	gss_delete_sec_context(&tmpmin, &sc->ctx_handle,
			       GSS_C_NO_BUFFER);

	 
	for (i = 0; i < sc->mech_set->count; i++) {
		if (g_OID_equal(supportedMech, &sc->mech_set->elements[i]))
			break;
	}
	if (i == sc->mech_set->count)
		return GSS_S_DEFECTIVE_TOKEN;
	sc->internal_mech = &sc->mech_set->elements[i];

	 
	if (acc_negState == ACCEPT_INCOMPLETE) {
		if (!g_OID_equal(supportedMech, &gss_mech_ntlmssp_oid))
			return GSS_S_DEFECTIVE_TOKEN;
	} else if (acc_negState != REQUEST_MIC) {
		return GSS_S_DEFECTIVE_TOKEN;
	}

	sc->mech_complete = 0;
	sc->mic_reqd = (acc_negState == REQUEST_MIC);
	*negState = acc_negState;
	*tokflag = CONT_TOKEN_SEND;
	return GSS_S_CONTINUE_NEEDED;
}