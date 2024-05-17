val_init_sec_ctx_args(
    OM_uint32 *minor_status,
    gss_cred_id_t claimant_cred_handle,
    gss_ctx_id_t *context_handle,
    gss_name_t target_name,
    gss_OID req_mech_type,
    OM_uint32 req_flags,
    OM_uint32 time_req,
    gss_channel_bindings_t input_chan_bindings,
    gss_buffer_t input_token,
    gss_OID *actual_mech_type,
    gss_buffer_t output_token,
    OM_uint32 *ret_flags,
    OM_uint32 *time_rec)
{

     

    if (minor_status != NULL)
	*minor_status = 0;

    if (actual_mech_type != NULL)
	*actual_mech_type = GSS_C_NO_OID;

    if (output_token != GSS_C_NO_BUFFER) {
	output_token->length = 0;
	output_token->value = NULL;
    }

     

    if (minor_status == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (context_handle == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE | GSS_S_NO_CONTEXT);

    if (target_name == NULL)
	return (GSS_S_CALL_INACCESSIBLE_READ | GSS_S_BAD_NAME);

    if (output_token == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    return (GSS_S_COMPLETE);
}