make_spnego_tokenTarg_msg(OM_uint32 status, gss_OID mech_wanted,
			  gss_buffer_t data, gss_buffer_t mechListMIC,
			  send_token_flag sendtoken,
			  gss_buffer_t outbuf)
{
	unsigned int tlen = 0;
	unsigned int ret = 0;
	unsigned int NegTokenTargSize = 0;
	unsigned int NegTokenSize = 0;
	unsigned int rspTokenSize = 0;
	unsigned int micTokenSize = 0;
	unsigned int dataLen = 0;
	unsigned char *t;
	unsigned char *ptr;

	if (outbuf == GSS_C_NO_BUFFER)
		return (GSS_S_DEFECTIVE_TOKEN);
	if (sendtoken == INIT_TOKEN_SEND && mech_wanted == GSS_C_NO_OID)
	    return (GSS_S_DEFECTIVE_TOKEN);

	outbuf->length = 0;
	outbuf->value = NULL;

	 
	dataLen = 5;

	 
	if (sendtoken == INIT_TOKEN_SEND) {
		int mechlistTokenSize;
		 
		mechlistTokenSize = 3 + mech_wanted->length +
			gssint_der_length_size(mech_wanted->length);

		dataLen += mechlistTokenSize;
	}
	if (data != NULL && data->length > 0) {
		 
		rspTokenSize = 1 + gssint_der_length_size(data->length) +
			data->length;

		dataLen += rspTokenSize;

		 
		dataLen += 1 + gssint_der_length_size(rspTokenSize);
	}
	if (mechListMIC != NULL) {

		 
		micTokenSize = 1 + gssint_der_length_size(mechListMIC->length) +
			mechListMIC->length;

		dataLen += micTokenSize;

		 
		dataLen += 1 + gssint_der_length_size(micTokenSize);
	}
	 
	NegTokenTargSize = dataLen;
	dataLen += 1 + gssint_der_length_size(NegTokenTargSize);

	 
	NegTokenSize = dataLen;
	dataLen += 1 + gssint_der_length_size(NegTokenSize);

	tlen = dataLen;
	t = (unsigned char *) gssalloc_malloc(tlen);

	if (t == NULL) {
		ret = GSS_S_DEFECTIVE_TOKEN;
		goto errout;
	}

	ptr = t;

	 
	*ptr++ = CONTEXT | 0x01;
	if (gssint_put_der_length(NegTokenSize, &ptr, dataLen) < 0) {
		ret = GSS_S_DEFECTIVE_TOKEN;
		goto errout;
	}
	*ptr++ = SEQUENCE;
	if (gssint_put_der_length(NegTokenTargSize, &ptr,
				  tlen - (int)(ptr-t)) < 0) {
		ret = GSS_S_DEFECTIVE_TOKEN;
		goto errout;
	}

	 
	*ptr++ = CONTEXT;
	if (gssint_put_der_length(3, &ptr,
				  tlen - (int)(ptr-t)) < 0) {
		ret = GSS_S_DEFECTIVE_TOKEN;
		goto errout;
	}
	if (put_negResult(&ptr, status, tlen - (int)(ptr - t)) < 0) {
		ret = GSS_S_DEFECTIVE_TOKEN;
		goto errout;
	}
	if (sendtoken == INIT_TOKEN_SEND) {
		 
		*ptr++ = CONTEXT | 0x01;
		if (gssint_put_der_length(mech_wanted->length + 2,
					  &ptr,
					  tlen - (int)(ptr - t)) < 0) {
			ret = GSS_S_DEFECTIVE_TOKEN;
			goto errout;
		}
		if (put_mech_oid(&ptr, mech_wanted,
				 tlen - (int)(ptr - t)) < 0) {
			ret = GSS_S_DEFECTIVE_TOKEN;
			goto errout;
		}
	}
	if (data != NULL && data->length > 0) {
		*ptr++ = CONTEXT | 0x02;
		if (gssint_put_der_length(rspTokenSize, &ptr,
					  tlen - (int)(ptr - t)) < 0) {
			ret = GSS_S_DEFECTIVE_TOKEN;
			goto errout;
		}
		if (put_input_token(&ptr, data,
				    tlen - (int)(ptr - t)) < 0) {
			ret = GSS_S_DEFECTIVE_TOKEN;
			goto errout;
		}
	}
	if (mechListMIC != NULL) {
		*ptr++ = CONTEXT | 0x03;
		if (gssint_put_der_length(micTokenSize, &ptr,
					  tlen - (int)(ptr - t)) < 0) {
			ret = GSS_S_DEFECTIVE_TOKEN;
			goto errout;
		}
		if (put_input_token(&ptr, mechListMIC,
				    tlen - (int)(ptr - t)) < 0) {
			ret = GSS_S_DEFECTIVE_TOKEN;
			goto errout;
		}
	}
	ret = GSS_S_COMPLETE;
errout:
	if (ret != GSS_S_COMPLETE) {
		if (t)
			free(t);
	} else {
		outbuf->length = ptr - t;
		outbuf->value = (void *) t;
	}

	return (ret);
}
