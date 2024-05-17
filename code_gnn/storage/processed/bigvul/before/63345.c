val_get_mic_args(
    OM_uint32 *minor_status,
    gss_ctx_id_t context_handle,
    gss_qop_t qop_req,
    gss_buffer_t message_buffer,
    gss_buffer_t msg_token)
{

     

    if (minor_status != NULL)
	*minor_status = 0;

    if (msg_token != GSS_C_NO_BUFFER) {
	msg_token->value = NULL;
	msg_token->length = 0;
    }

     

    if (minor_status == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (context_handle == GSS_C_NO_CONTEXT)
	return (GSS_S_CALL_INACCESSIBLE_READ | GSS_S_NO_CONTEXT);

    if (message_buffer == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_READ);

    if (msg_token == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    return (GSS_S_COMPLETE);
}