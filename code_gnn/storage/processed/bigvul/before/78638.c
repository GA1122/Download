piv_get_data(sc_card_t * card, int enumtag, u8 **buf, size_t *buf_len)
{
	u8 *p;
	int r = 0;
	u8 tagbuf[8];
	size_t tag_len;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	sc_log(card->ctx, "#%d", enumtag);

	sc_lock(card);  

	 

	tag_len = piv_objects[enumtag].tag_len;

	p = tagbuf;
	put_tag_and_len(0x5c, tag_len, &p);
	memcpy(p, piv_objects[enumtag].tag_value, tag_len);
	p += tag_len;

	if (*buf_len == 1 && *buf == NULL) {  
		u8 rbufinitbuf[8];  
		u8 *rbuf;
		size_t rbuflen;
		size_t bodylen;
		unsigned int cla_out, tag_out;
		const u8 *body;

		sc_log(card->ctx, "get len of #%d", enumtag);
		rbuf = rbufinitbuf;
		rbuflen = sizeof(rbufinitbuf);
		r = piv_general_io(card, 0xCB, 0x3F, 0xFF, tagbuf,  p - tagbuf, &rbuf, &rbuflen);
		if (r > 0) {
			body = rbuf;
			if (sc_asn1_read_tag(&body, 0xffff, &cla_out, &tag_out, &bodylen) !=  SC_SUCCESS
					|| body == NULL) {
				sc_log(card->ctx, "***** received buffer tag MISSING ");
				r = SC_ERROR_FILE_NOT_FOUND;
				goto err;
			}
		    *buf_len = r;
		} else if ( r == 0 ) {
			r = SC_ERROR_FILE_NOT_FOUND;
			goto err;
		} else {
			goto err;
		}
	}

	sc_log(card->ctx,
	       "buffer for #%d *buf=0x%p len=%"SC_FORMAT_LEN_SIZE_T"u",
	       enumtag, *buf, *buf_len);
	if (*buf == NULL && *buf_len > 0) {
		*buf = malloc(*buf_len);
		if (*buf == NULL ) {
			r = SC_ERROR_OUT_OF_MEMORY;
			goto err;
		}
	}

	r = piv_general_io(card, 0xCB, 0x3F, 0xFF, tagbuf,  p - tagbuf, buf, buf_len);

err:
	sc_unlock(card);
	LOG_FUNC_RETURN(card->ctx, r);
}