const u8 *sc_asn1_skip_tag(sc_context_t *ctx, const u8 ** buf, size_t *buflen,
			   unsigned int tag_in, size_t *taglen_out)
{
	const u8 *p = *buf;
	size_t len = *buflen, taglen;
	unsigned int cla = 0, tag;

	if (sc_asn1_read_tag((const u8 **) &p, len, &cla, &tag, &taglen) != SC_SUCCESS
			|| p == NULL)
		return NULL;
	switch (cla & 0xC0) {
	case SC_ASN1_TAG_UNIVERSAL:
		if ((tag_in & SC_ASN1_CLASS_MASK) != SC_ASN1_UNI)
			return NULL;
		break;
	case SC_ASN1_TAG_APPLICATION:
		if ((tag_in & SC_ASN1_CLASS_MASK) != SC_ASN1_APP)
			return NULL;
		break;
	case SC_ASN1_TAG_CONTEXT:
		if ((tag_in & SC_ASN1_CLASS_MASK) != SC_ASN1_CTX)
			return NULL;
		break;
	case SC_ASN1_TAG_PRIVATE:
		if ((tag_in & SC_ASN1_CLASS_MASK) != SC_ASN1_PRV)
			return NULL;
		break;
	}
	if (cla & SC_ASN1_TAG_CONSTRUCTED) {
		if ((tag_in & SC_ASN1_CONS) == 0)
			return NULL;
	} else
		if (tag_in & SC_ASN1_CONS)
			return NULL;
	if ((tag_in & SC_ASN1_TAG_MASK) != tag)
		return NULL;
	len -= (p - *buf);	 
	if (taglen > len) {
		sc_debug(ctx, SC_LOG_DEBUG_ASN1,
			 "too long ASN.1 object (size %"SC_FORMAT_LEN_SIZE_T"u while only %"SC_FORMAT_LEN_SIZE_T"u available)\n",
			 taglen, len);
		return NULL;
	}
	*buflen -= (p - *buf) + taglen;
	*buf = p + taglen;	 
	*taglen_out = taglen;
	return p;
}