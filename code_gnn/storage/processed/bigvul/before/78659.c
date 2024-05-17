piv_write_certificate(sc_card_t *card, const u8* buf, size_t count, unsigned long flags)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int enumtag;
	int r = SC_SUCCESS;
	u8 *sbuf = NULL;
	u8 *p;
	size_t sbuflen;
	size_t taglen;

	sc_log(card->ctx, "DEE cert len=%"SC_FORMAT_LEN_SIZE_T"u", count);
	taglen = put_tag_and_len(0x70, count, NULL)
		+ put_tag_and_len(0x71, 1, NULL)
		+ put_tag_and_len(0xFE, 0, NULL);

	sbuflen =  put_tag_and_len(0x53, taglen, NULL);

	sbuf = malloc(sbuflen);
	if (sbuf == NULL)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
	p = sbuf;
	put_tag_and_len(0x53, taglen, &p);

	put_tag_and_len(0x70, count, &p);
	memcpy(p, buf, count);
	p += count;
	put_tag_and_len(0x71, 1, &p);
	 
	*p++ = (flags)? 0x01:0x00;  
	put_tag_and_len(0xFE,0,&p);  

	sc_log(card->ctx, "DEE buf %p len %"SC_FORMAT_LEN_PTRDIFF_T"u %"SC_FORMAT_LEN_SIZE_T"u",
	       sbuf, p - sbuf, sbuflen);

	enumtag = piv_objects[priv->selected_obj].enumtag;
	r = piv_put_data(card, enumtag, sbuf, sbuflen);
	if (sbuf)
		free(sbuf);

	LOG_FUNC_RETURN(card->ctx, r);
}