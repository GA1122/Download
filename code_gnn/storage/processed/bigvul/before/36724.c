make_spnego_tokenInit_msg(spnego_gss_ctx_id_t spnego_ctx,
			  int negHintsCompat,
			  gss_buffer_t mechListMIC, OM_uint32 req_flags,
			  gss_buffer_t data, send_token_flag sendtoken,
			  gss_buffer_t outbuf)
{
	int ret = 0;
	unsigned int tlen, dataLen = 0;
	unsigned int negTokenInitSize = 0;
	unsigned int negTokenInitSeqSize = 0;
	unsigned int negTokenInitContSize = 0;
	unsigned int rspTokenSize = 0;
	unsigned int mechListTokenSize = 0;
	unsigned int micTokenSize = 0;
	unsigned char *t;
	unsigned char *ptr;

	if (outbuf == GSS_C_NO_BUFFER)
		return (-1);

	outbuf->length = 0;
	outbuf->value = NULL;

	 

	 
	mechListTokenSize = 1 +
		gssint_der_length_size(spnego_ctx->DER_mechTypes.length) +
		spnego_ctx->DER_mechTypes.length;
	dataLen += mechListTokenSize;

	 
	if (data != NULL) {
		 
		rspTokenSize = 1 +
			gssint_der_length_size(data->length) +
			data->length;
		dataLen += 1 + gssint_der_length_size(rspTokenSize) +
			rspTokenSize;
	}

	if (mechListMIC) {
		 
		micTokenSize = 1 +
			gssint_der_length_size(mechListMIC->length) +
			mechListMIC->length;
		dataLen += 1 +
			gssint_der_length_size(micTokenSize) +
			micTokenSize;
	}

	 
	negTokenInitContSize = dataLen;
	negTokenInitSeqSize = 1 + gssint_der_length_size(dataLen) + dataLen;
	dataLen = negTokenInitSeqSize;

	 
	negTokenInitSize = 1 +
		gssint_der_length_size(negTokenInitSeqSize) +
		negTokenInitSeqSize;

	tlen = g_token_size(gss_mech_spnego, negTokenInitSize);

	t = (unsigned char *) gssalloc_malloc(tlen);

	if (t == NULL) {
		return (-1);
	}

	ptr = t;

	 
	if ((ret = g_make_token_header(gss_mech_spnego, negTokenInitSize,
			    &ptr, tlen)))
		goto errout;

	*ptr++ = CONTEXT;  
	if ((ret = gssint_put_der_length(negTokenInitSeqSize, &ptr, tlen)))
		goto errout;

	*ptr++ = SEQUENCE;
	if ((ret = gssint_put_der_length(negTokenInitContSize, &ptr,
					 tlen - (int)(ptr-t))))
		goto errout;

	*ptr++ = CONTEXT | 0x00;  
	if ((ret = gssint_put_der_length(spnego_ctx->DER_mechTypes.length,
					 &ptr, tlen - (int)(ptr-t))))
		goto errout;

	 
	(void) memcpy(ptr, spnego_ctx->DER_mechTypes.value,
		      spnego_ctx->DER_mechTypes.length);

	ptr += spnego_ctx->DER_mechTypes.length;

	if (data != NULL) {
		*ptr++ = CONTEXT | 0x02;
		if ((ret = gssint_put_der_length(rspTokenSize,
				&ptr, tlen - (int)(ptr - t))))
			goto errout;

		if ((ret = put_input_token(&ptr, data,
			tlen - (int)(ptr - t))))
			goto errout;
	}

	if (mechListMIC != GSS_C_NO_BUFFER) {
		*ptr++ = CONTEXT | 0x03;
		if ((ret = gssint_put_der_length(micTokenSize,
				&ptr, tlen - (int)(ptr - t))))
			goto errout;

		if (negHintsCompat) {
			ret = put_neg_hints(&ptr, mechListMIC,
					    tlen - (int)(ptr - t));
			if (ret)
				goto errout;
		} else if ((ret = put_input_token(&ptr, mechListMIC,
				tlen - (int)(ptr - t))))
			goto errout;
	}

errout:
	if (ret != 0) {
		if (t)
			free(t);
		t = NULL;
		tlen = 0;
	}
	outbuf->length = tlen;
	outbuf->value = (void *) t;

	return (ret);
}
