static int piv_parse_discovery(sc_card_t *card, u8 * rbuf, size_t rbuflen, int aid_only)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int r = 0;
	const u8 * body;
	size_t bodylen;
	const u8 * aid;
	size_t aidlen;
	const u8 * pinp;
	size_t pinplen;
	unsigned int cla_out, tag_out;


	if (rbuflen != 0) {
		body = rbuf;
		if ((r = sc_asn1_read_tag(&body, rbuflen, &cla_out, &tag_out,  &bodylen)) != SC_SUCCESS) {
			sc_log(card->ctx, "DER problem %d",r);
			r = SC_ERROR_INVALID_ASN1_OBJECT;
			goto err;
		}

	sc_log(card->ctx,
	       "Discovery 0x%2.2x 0x%2.2x %p:%"SC_FORMAT_LEN_SIZE_T"u",
	       cla_out, tag_out, body, bodylen);
	if ( cla_out+tag_out == 0x7E && body != NULL && bodylen != 0) {
		aidlen = 0;
		aid = sc_asn1_find_tag(card->ctx, body, bodylen, 0x4F, &aidlen);
		sc_log(card->ctx, "Discovery aid=%p:%"SC_FORMAT_LEN_SIZE_T"u",
		       aid, aidlen);
			if (aid == NULL || aidlen < piv_aids[0].len_short ||
				memcmp(aid,piv_aids[0].value,piv_aids[0].len_short) != 0) {  
				sc_log(card->ctx, "Discovery object not PIV");
				r = SC_ERROR_INVALID_CARD;  
				goto err;
			}
			if (aid_only == 0) {
				pinp = sc_asn1_find_tag(card->ctx, body, bodylen, 0x5F2F, &pinplen);
				sc_log(card->ctx,
				       "Discovery pinp=%p:%"SC_FORMAT_LEN_SIZE_T"u",
				       pinp, pinplen);
				if (pinp && pinplen == 2) {
					sc_log(card->ctx, "Discovery pinp flags=0x%2.2x 0x%2.2x",*pinp, *(pinp+1));
					r = SC_SUCCESS;
					if (*pinp == 0x60 && *(pinp+1) == 0x20) {  
						sc_log(card->ctx, "Pin Preference - Global");
						priv->pin_preference = 0x00;
					}
				}
			}
		}
	}

err:
	LOG_FUNC_RETURN(card->ctx, r);
}