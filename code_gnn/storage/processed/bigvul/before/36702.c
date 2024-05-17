g_verify_neg_token_init(unsigned char **buf_in, unsigned int cur_size)
{
	unsigned char *buf = *buf_in;
	unsigned char *endptr = buf + cur_size;
	int seqsize;
	int ret = 0;
	unsigned int bytes;

	 
	if (g_get_tag_and_length(&buf, CONTEXT, cur_size, &bytes) < 0)
		return (G_BAD_TOK_HEADER);

	cur_size = bytes;  

	 
	if (*buf++ == SEQUENCE) {
		if ((seqsize = gssint_get_der_length(&buf, cur_size, &bytes)) < 0)
			return (G_BAD_TOK_HEADER);
		 
		if (seqsize > endptr - buf)
			return (G_BAD_TOK_HEADER);
	} else {
		return (G_BAD_TOK_HEADER);
	}

	cur_size = seqsize;  

	 
	if (*buf++ == CONTEXT) {
		if ((seqsize = gssint_get_der_length(&buf, cur_size, &bytes)) < 0)
			return (G_BAD_TOK_HEADER);
		 
		if (seqsize > endptr - buf)
			return (G_BAD_TOK_HEADER);
	} else {
		return (G_BAD_TOK_HEADER);
	}

	 
	*buf_in = buf;

	return (ret);

}
