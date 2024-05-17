gssint_unwrap_aead (gss_mechanism mech,
		    OM_uint32 *minor_status,
		    gss_union_ctx_id_t ctx,
		    gss_buffer_t input_message_buffer,
		    gss_buffer_t input_assoc_buffer,
		    gss_buffer_t output_payload_buffer,
		    int *conf_state,
		    gss_qop_t *qop_state)
{
    OM_uint32		    status;

    assert(mech != NULL);
    assert(ctx != NULL);

  

    if (mech->gss_unwrap_aead) {
	status = mech->gss_unwrap_aead(minor_status,
				       ctx->internal_ctx_id,
				       input_message_buffer,
				       input_assoc_buffer,
				       output_payload_buffer,
				       conf_state,
				       qop_state);
	if (status != GSS_S_COMPLETE)
	    map_error(minor_status, mech);
    } else if (mech->gss_unwrap_iov) {
	status = gssint_unwrap_aead_iov_shim(mech,
					     minor_status,
					     ctx->internal_ctx_id,
					     input_message_buffer,
					     input_assoc_buffer,
					     output_payload_buffer,
					     conf_state,
					     qop_state);
    } else
	status = GSS_S_UNAVAILABLE;
  

    return (status);
}