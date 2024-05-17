static int piv_validate_general_authentication(sc_card_t *card,
					const u8 * data, size_t datalen,
					u8 * out, size_t outlen)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int r;
	u8 *p;
	const u8 *tag;
	size_t taglen;
	const u8 *body;
	size_t bodylen;
	unsigned int real_alg_id;

	u8 sbuf[4096];  
	u8 *rbuf = NULL;
	size_t rbuflen = 0;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 
	p = sbuf;
	put_tag_and_len(0x7c, (2 + put_tag_and_len(0, datalen, NULL)) , &p);
	put_tag_and_len(0x82, 0, &p);
	if (priv->operation == SC_SEC_OPERATION_DERIVE
			&& priv->algorithm == SC_ALGORITHM_EC)
		put_tag_and_len(0x85, datalen, &p);
	else
		put_tag_and_len(0x81, datalen, &p);

	memcpy(p, data, datalen);
	p += datalen;

	 

	real_alg_id = priv->alg_id;
	if (priv->alg_id == 0x06) {
		switch  (datalen) {
			case 128: real_alg_id = 0x06; break;
			case 256: real_alg_id = 0x07; break;
			case 384: real_alg_id = 0x05; break;
			default:
				SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_ERROR_NO_CARD_SUPPORT);
		}
	}
	 

	r = piv_general_io(card, 0x87, real_alg_id, priv->key_ref,
			sbuf, p - sbuf, &rbuf, &rbuflen);

	if ( r >= 0) {
	 	body = sc_asn1_find_tag(card->ctx, rbuf, rbuflen, 0x7c, &bodylen);

		if (body) {
			tag = sc_asn1_find_tag(card->ctx, body,  bodylen, 0x82, &taglen);
			if (tag) {
				memcpy(out, tag, taglen);
				r = taglen;
			}
		} else
			r = SC_ERROR_INVALID_DATA;
	}

	if (rbuf)
		free(rbuf);

	LOG_FUNC_RETURN(card->ctx, r);
}