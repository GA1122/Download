spnego_gss_set_cred_option(
		OM_uint32 *minor_status,
		gss_cred_id_t *cred_handle,
		const gss_OID desired_object,
		const gss_buffer_t value)
{
	OM_uint32 ret;
	OM_uint32 tmp_minor_status;
	spnego_gss_cred_id_t spcred = (spnego_gss_cred_id_t)*cred_handle;
	gss_cred_id_t mcred;

	mcred = (spcred == NULL) ? GSS_C_NO_CREDENTIAL : spcred->mcred;
	ret = gss_set_cred_option(minor_status,
				  &mcred,
				  desired_object,
				  value);
	if (ret == GSS_S_COMPLETE && spcred == NULL) {
		 

		ret = create_spnego_cred(minor_status, mcred, &spcred);
		if (ret != GSS_S_COMPLETE) {
			gss_release_cred(&tmp_minor_status, &mcred);
			return (ret);
		}
		*cred_handle = (gss_cred_id_t)spcred;
	}

	if (ret != GSS_S_COMPLETE)
		return (ret);

	 
	if (g_OID_equal(desired_object, no_ci_flags_oid))
		spcred->no_ask_integ = 1;

	return (GSS_S_COMPLETE);
}