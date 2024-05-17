val_wrap_aead_args(
    OM_uint32 *minor_status,
    gss_ctx_id_t context_handle,
    int conf_req_flag,
    gss_qop_t qop_req,
    gss_buffer_t input_assoc_buffer,
    gss_buffer_t input_payload_buffer,
    int *conf_state,
    gss_buffer_t output_message_buffer)
{

     

    if (minor_status != NULL)
	*minor_status = 0;

     

    if (minor_status == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (context_handle == GSS_C_NO_CONTEXT)
	return (GSS_S_CALL_INACCESSIBLE_READ | GSS_S_NO_CONTEXT);

    if (input_payload_buffer == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_READ);

    if (output_message_buffer == GSS_C_NO_BUFFER)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    return (GSS_S_COMPLETE);
}