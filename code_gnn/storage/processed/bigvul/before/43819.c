spnego_gss_acquire_cred_from(OM_uint32 *minor_status,
			     const gss_name_t desired_name,
			     OM_uint32 time_req,
			     const gss_OID_set desired_mechs,
			     gss_cred_usage_t cred_usage,
			     gss_const_key_value_set_t cred_store,
			     gss_cred_id_t *output_cred_handle,
			     gss_OID_set *actual_mechs,
			     OM_uint32 *time_rec)
{
	OM_uint32 status, tmpmin;
	gss_OID_set amechs;
	gss_cred_id_t mcred = NULL;
	spnego_gss_cred_id_t spcred = NULL;
	dsyslog("Entering spnego_gss_acquire_cred\n");

	if (actual_mechs)
		*actual_mechs = NULL;

	if (time_rec)
		*time_rec = 0;

	 
	status = create_spnego_cred(minor_status, mcred, &spcred);
	if (status != GSS_S_COMPLETE)
		return (status);

	 
	status = get_available_mechs(minor_status, desired_name,
				     cred_usage, cred_store, &mcred,
				     &amechs, time_rec);

	if (actual_mechs && amechs != GSS_C_NULL_OID_SET) {
		(void) gssint_copy_oid_set(&tmpmin, amechs, actual_mechs);
	}
	(void) gss_release_oid_set(&tmpmin, &amechs);

	if (status == GSS_S_COMPLETE) {
		spcred->mcred = mcred;
		*output_cred_handle = (gss_cred_id_t)spcred;
	} else {
		free(spcred);
		*output_cred_handle = GSS_C_NO_CREDENTIAL;
	}

	dsyslog("Leaving spnego_gss_acquire_cred\n");
	return (status);
}