gssint_wrap_aead_iov_shim(gss_mechanism mech,
			  OM_uint32 *minor_status,
			  gss_ctx_id_t context_handle,
			  int conf_req_flag,
			  gss_qop_t qop_req,
			  gss_buffer_t input_assoc_buffer,
			  gss_buffer_t input_payload_buffer,
			  int *conf_state,
			  gss_buffer_t output_message_buffer)
{
    gss_iov_buffer_desc	iov[5];
    OM_uint32		status;
    size_t		offset;
    int			i = 0, iov_count;

     

    iov[i].type = GSS_IOV_BUFFER_TYPE_HEADER;
    iov[i].buffer.value = NULL;
    iov[i].buffer.length = 0;
    i++;

    if (input_assoc_buffer != GSS_C_NO_BUFFER) {
	iov[i].type = GSS_IOV_BUFFER_TYPE_SIGN_ONLY;
	iov[i].buffer = *input_assoc_buffer;
	i++;
    }

    iov[i].type = GSS_IOV_BUFFER_TYPE_DATA;
    iov[i].buffer = *input_payload_buffer;
    i++;

    iov[i].type = GSS_IOV_BUFFER_TYPE_PADDING;
    iov[i].buffer.value = NULL;
    iov[i].buffer.length = 0;
    i++;

    iov[i].type = GSS_IOV_BUFFER_TYPE_TRAILER;
    iov[i].buffer.value = NULL;
    iov[i].buffer.length = 0;
    i++;

    iov_count = i;

    assert(mech->gss_wrap_iov_length);

    status = mech->gss_wrap_iov_length(minor_status, context_handle,
				       conf_req_flag, qop_req,
				       NULL, iov, iov_count);
    if (status != GSS_S_COMPLETE) {
	map_error(minor_status, mech);
	return status;
    }

     
    for (i = 0, output_message_buffer->length = 0; i < iov_count; i++) {
	if (GSS_IOV_BUFFER_TYPE(iov[i].type) == GSS_IOV_BUFFER_TYPE_SIGN_ONLY)
	    continue;

	output_message_buffer->length += iov[i].buffer.length;
    }

    output_message_buffer->value = gssalloc_malloc(output_message_buffer->length);
    if (output_message_buffer->value == NULL) {
	*minor_status = ENOMEM;
	return GSS_S_FAILURE;
    }

    i = 0, offset = 0;

     
    iov[i].buffer.value = (unsigned char *)output_message_buffer->value + offset;
    offset += iov[i].buffer.length;
    i++;

     
    if (input_assoc_buffer != GSS_C_NO_BUFFER)
	i++;

     
    iov[i].buffer.value = (unsigned char *)output_message_buffer->value + offset;
    offset += iov[i].buffer.length;

    memcpy(iov[i].buffer.value, input_payload_buffer->value, iov[i].buffer.length);
    i++;

     
    iov[i].buffer.value = (unsigned char *)output_message_buffer->value + offset;
    offset += iov[i].buffer.length;
    i++;

     
    iov[i].buffer.value = (unsigned char *)output_message_buffer->value + offset;
    offset += iov[i].buffer.length;
    i++;

    assert(offset == output_message_buffer->length);

    assert(mech->gss_wrap_iov);

    status = mech->gss_wrap_iov(minor_status, context_handle,
				conf_req_flag, qop_req,
				conf_state, iov, iov_count);
    if (status != GSS_S_COMPLETE) {
	OM_uint32 minor;

	map_error(minor_status, mech);
	gss_release_buffer(&minor, output_message_buffer);
    }

    return status;
}