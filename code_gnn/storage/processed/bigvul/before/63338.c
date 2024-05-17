val_exp_sec_ctx_args(
    OM_uint32 *minor_status,
    gss_ctx_id_t *context_handle,
    gss_buffer_t interprocess_token)
{

     

    if (minor_status != NULL)
	*minor_status = 0;

    if (interprocess_token != GSS_C_NO_BUFFER) {
	interprocess_token->length = 0;
	interprocess_token->value = NULL;
    }

     

    if (minor_status == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (context_handle == NULL || *context_handle == GSS_C_NO_CONTEXT)
	return (GSS_S_CALL_INACCESSIBLE_READ | GSS_S_NO_CONTEXT);

    if (interprocess_token == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    return (GSS_S_COMPLETE);
}