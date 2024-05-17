val_acc_sec_ctx_args(
    OM_uint32 *minor_status,
    gss_ctx_id_t *context_handle,
    gss_cred_id_t verifier_cred_handle,
    gss_buffer_t input_token_buffer,
    gss_channel_bindings_t input_chan_bindings,
    gss_name_t *src_name,
    gss_OID *mech_type,
    gss_buffer_t output_token,
    OM_uint32 *ret_flags,
    OM_uint32 *time_rec,
    gss_cred_id_t *d_cred)
{

     

    if (minor_status != NULL)
	*minor_status = 0;

    if (src_name != NULL)
	*src_name = GSS_C_NO_NAME;

    if (mech_type != NULL)
	*mech_type = GSS_C_NO_OID;

    if (output_token != GSS_C_NO_BUFFER) {
	output_token->length = 0;
	output_token->value = NULL;
    }

    if (d_cred != NULL)
	*d_cred = GSS_C_NO_CREDENTIAL;

     

    if (minor_status == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (context_handle == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (input_token_buffer == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_READ);

    if (output_token == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    return (GSS_S_COMPLETE);
}