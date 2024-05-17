piv_process_history(sc_card_t *card)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int r;
	int i;
	int enumtag;
	u8 * rbuf = NULL;
	size_t rbuflen = 0;
	const u8 * body;
	size_t bodylen;
	const u8 * num;
	size_t numlen;
	const u8 * url = NULL;
	size_t urllen;
	u8 * ocfhfbuf = NULL;
	unsigned int cla_out, tag_out;
	size_t ocfhflen;
	const u8 * seq;
	const u8 * seqtag;
	size_t seqlen;
	const u8 * keyref;
	size_t keyreflen;
	const u8 * cert;
	size_t certlen;
	size_t certobjlen, i2;
	u8 * certobj;
	u8 * cp;


	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	r = piv_get_cached_data(card, PIV_OBJ_HISTORY, &rbuf, &rbuflen);
	if (r == SC_ERROR_FILE_NOT_FOUND)
		r = 0;			 
	if (r <= 0) {
		priv->obj_cache[PIV_OBJ_HISTORY].flags |= PIV_OBJ_CACHE_NOT_PRESENT;
		goto err;		 
	}

	 
	if (rbuflen != 0) {
		body = rbuf;
		if ((r = sc_asn1_read_tag(&body, rbuflen, &cla_out, &tag_out,  &bodylen)) != SC_SUCCESS) {
			sc_log(card->ctx, "DER problem %d",r);
			r = SC_ERROR_INVALID_ASN1_OBJECT;
			goto err;
		}

		if ( cla_out+tag_out == 0x53 && body != NULL && bodylen != 0) {
			numlen = 0;
			num = sc_asn1_find_tag(card->ctx, body, bodylen, 0xC1, &numlen);
			if (num) {
				if (numlen != 1 || *num > PIV_OBJ_RETIRED_X509_20-PIV_OBJ_RETIRED_X509_1+1) {
					r = SC_ERROR_INTERNAL;  
					goto err;
				}

				priv->keysWithOnCardCerts = *num;
			}

			numlen = 0;
			num = sc_asn1_find_tag(card->ctx, body, bodylen, 0xC2, &numlen);
			if (num) {
				if (numlen != 1 || *num > PIV_OBJ_RETIRED_X509_20-PIV_OBJ_RETIRED_X509_1+1) {
					r = SC_ERROR_INTERNAL;  
					goto err;
				}

				priv->keysWithOffCardCerts = *num;
			}

			url = sc_asn1_find_tag(card->ctx, body, bodylen, 0xF3, &urllen);
			if (url) {
				priv->offCardCertURL = calloc(1,urllen+1);
				if (priv->offCardCertURL == NULL)
					LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
				memcpy(priv->offCardCertURL, url, urllen);
			}
		}
		else {
			sc_log(card->ctx, "Problem with History object\n");
			goto err;
		}
	}
	sc_log(card->ctx, "History on=%d off=%d URL=%s",
			priv->keysWithOnCardCerts, priv->keysWithOffCardCerts,
			priv->offCardCertURL ? priv->offCardCertURL:"NONE");

	 
	for (i=0; i<priv->keysWithOnCardCerts; i++)
		priv->obj_cache[PIV_OBJ_RETIRED_X509_1+i].flags &= ~PIV_OBJ_CACHE_NOT_PRESENT;

	 

	r = 0;
	if (priv->offCardCertURL) {
		char * fp;
		char filename[PATH_MAX];

		if (strncmp("http://", priv->offCardCertURL, 7)) {
			r = SC_ERROR_INVALID_DATA;
			goto err;
		}
		 
		fp = strrchr(priv->offCardCertURL + 7,'/');
		if (fp == NULL) {
			r = SC_ERROR_INVALID_DATA;
			goto err;
		}
		fp++;

		 
		r = sc_get_cache_dir(card->ctx, filename, sizeof(filename) - strlen(fp) - 2);
		if (r != SC_SUCCESS)
			goto err;
#ifdef _WIN32
		strcat(filename,"\\");
#else
		strcat(filename,"/");
#endif
		strcat(filename,fp);

		r = piv_read_obj_from_file(card, filename,
			 &ocfhfbuf, &ocfhflen);
		if (r == SC_ERROR_FILE_NOT_FOUND) {
			r = 0;
			goto err;
		}

		 

		body = ocfhfbuf;
		if (sc_asn1_read_tag(&body, ocfhflen, &cla_out,
					&tag_out, &bodylen) != SC_SUCCESS
				|| cla_out+tag_out != 0x30) {
			sc_log(card->ctx, "DER problem");
			r = SC_ERROR_INVALID_ASN1_OBJECT;
			goto err;
		}
		seq = body;
		while (bodylen > 0) {
			seqtag = seq;
			if (sc_asn1_read_tag(&seq, bodylen, &cla_out,
						&tag_out, &seqlen) != SC_SUCCESS
					|| cla_out+tag_out != 0x30) {
				sc_log(card->ctx, "DER problem");
				r = SC_ERROR_INVALID_ASN1_OBJECT;
				goto err;
			}
			keyref = sc_asn1_find_tag(card->ctx, seq, seqlen, 0x04, &keyreflen);
			if (!keyref || keyreflen != 1 ||
					(*keyref < 0x82 || *keyref > 0x95)) {
				sc_log(card->ctx, "DER problem");
				r = SC_ERROR_INVALID_ASN1_OBJECT;
				goto err;
			}
			cert = keyref + keyreflen;
			certlen = seqlen - (cert - seq);

			enumtag = PIV_OBJ_RETIRED_X509_1 + *keyref - 0x82;
			 

			i2 = put_tag_and_len(0x70,certlen, NULL)
					+ put_tag_and_len(0x71, 1, NULL)
					+ put_tag_and_len(0xFE, 0, NULL);

			certobjlen = put_tag_and_len(0x53, i2, NULL);

			certobj = malloc(certobjlen);
			if (certobj == NULL) {
				r = SC_ERROR_OUT_OF_MEMORY;
				goto err;
			}
			cp = certobj;
			put_tag_and_len(0x53, i2, &cp);
			put_tag_and_len(0x70,certlen, &cp);
			memcpy(cp, cert, certlen);
			cp += certlen;
			put_tag_and_len(0x71, 1,&cp);
			*cp++ = 0x00;
			put_tag_and_len(0xFE, 0, &cp);

			priv->obj_cache[enumtag].obj_data = certobj;
			priv->obj_cache[enumtag].obj_len = certobjlen;
			priv->obj_cache[enumtag].flags |= PIV_OBJ_CACHE_VALID;
			priv->obj_cache[enumtag].flags &= ~PIV_OBJ_CACHE_NOT_PRESENT;

			r = piv_cache_internal_data(card, enumtag);
			sc_log(card->ctx, "got internal r=%d",r);

			certobj = NULL;

			sc_log(card->ctx,
			       "Added from off card file #%d %p:%"SC_FORMAT_LEN_SIZE_T"u 0x%02X",
			       enumtag,
			       priv->obj_cache[enumtag].obj_data,
			       priv->obj_cache[enumtag].obj_len, *keyref);

			bodylen -= (seqlen + seq - seqtag);
			seq += seqlen;
		}
	}
err:
	if (ocfhfbuf)
		free(ocfhfbuf);
	LOG_FUNC_RETURN(card->ctx, r);
}