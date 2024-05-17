dtls1_hm_fragment_new(unsigned long frag_len, int reassembly)
	{
	hm_fragment *frag = NULL;
	unsigned char *buf = NULL;
	unsigned char *bitmask = NULL;

	frag = (hm_fragment *)OPENSSL_malloc(sizeof(hm_fragment));
	if ( frag == NULL)
		return NULL;

	if (frag_len)
		{
		buf = (unsigned char *)OPENSSL_malloc(frag_len);
		if ( buf == NULL)
			{
			OPENSSL_free(frag);
			return NULL;
			}
		}

	 
	frag->fragment = buf;

	 
	if (reassembly)
		{
		bitmask = (unsigned char *)OPENSSL_malloc(RSMBLY_BITMASK_SIZE(frag_len));
		if (bitmask == NULL)
			{
			if (buf != NULL) OPENSSL_free(buf);
			OPENSSL_free(frag);
			return NULL;
			}
		memset(bitmask, 0, RSMBLY_BITMASK_SIZE(frag_len));
		}

	frag->reassembly = bitmask;

	return frag;
	}
