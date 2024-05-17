static int piv_read_obj_from_file(sc_card_t * card, char * filename,
	u8 **buf, size_t *buf_len)
{
	int r;
	int f = -1;
	size_t len;
	u8 tagbuf[16];
	size_t rbuflen;
	const u8 * body;
	unsigned int cla_out, tag_out;
	size_t bodylen;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	*buf = NULL;
	*buf_len = 0;
	f = open(filename, O_RDONLY);
	if (f < 0) {
		sc_log(card->ctx, "Unable to load PIV off card file: \"%s\"",filename);
			r = SC_ERROR_FILE_NOT_FOUND;
			goto err;
	}
	len = read(f, tagbuf, sizeof(tagbuf));  
	if (len < 2 || len > sizeof(tagbuf)) {
		sc_log(card->ctx, "Problem with \"%s\"",filename);
		r =  SC_ERROR_DATA_OBJECT_NOT_FOUND;
		goto err;
	}
	body = tagbuf;
	if (sc_asn1_read_tag(&body, 0xfffff, &cla_out, &tag_out, &bodylen) != SC_SUCCESS
			|| body == NULL) {
		sc_log(card->ctx, "DER problem");
		r = SC_ERROR_INVALID_ASN1_OBJECT;
		goto err;
	}
	rbuflen = body - tagbuf + bodylen;
	*buf = malloc(rbuflen);
	if (!*buf) {
		r = SC_ERROR_OUT_OF_MEMORY;
		goto err;
	}
	memcpy(*buf, tagbuf, len);  
	if (rbuflen > len + sizeof(tagbuf)) {
		len = read(f, *buf + sizeof(tagbuf), rbuflen - sizeof(tagbuf));  
		if (len != rbuflen - sizeof(tagbuf)) {
			r = SC_ERROR_INVALID_ASN1_OBJECT;
			free (*buf);
			*buf = NULL;
			goto err;
		}
	}
	r = rbuflen;
	*buf_len = rbuflen;
err:
	if (f >= 0)
		close(f);
	LOG_FUNC_RETURN(card->ctx, r);
}