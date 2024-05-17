gssint_wrap_aead (gss_mechanism mech,
		  OM_uint32 *minor_status,
		  gss_union_ctx_id_t ctx,
		  int conf_req_flag,
		  gss_qop_t qop_req,
		  gss_buffer_t input_assoc_buffer,
		  gss_buffer_t input_payload_buffer,
		  int *conf_state,
		  gss_buffer_t output_message_buffer)
{
  
    OM_uint32		status;

    assert(ctx != NULL);
    assert(mech != NULL);

    if (mech->gss_wrap_aead) {
	status = mech->gss_wrap_aead(minor_status,
				     ctx->internal_ctx_id,
				     conf_req_flag,
				     qop_req,
				     input_assoc_buffer,
				     input_payload_buffer,
				     conf_state,
				     output_message_buffer);
	if (status != GSS_S_COMPLETE)
	    map_error(minor_status, mech);
    } else if (mech->gss_wrap_iov && mech->gss_wrap_iov_length) {
	status = gssint_wrap_aead_iov_shim(mech,
					   minor_status,
					   ctx->internal_ctx_id,
					   conf_req_flag,
					   qop_req,
					   input_assoc_buffer,
					   input_payload_buffer,
					   conf_state,
					   output_message_buffer);
    } else
	status = GSS_S_UNAVAILABLE;

  

    return status;
}