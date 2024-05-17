static int piv_general_external_authenticate(sc_card_t *card,
		unsigned int key_ref, unsigned int alg_id)
{
	int r;
#ifdef ENABLE_OPENSSL
	int tmplen;
	int outlen;
	int locked = 0;
	u8 *p;
	u8 *rbuf = NULL;
	u8 *key = NULL;
	u8 *cypher_text = NULL;
	u8 *output_buf = NULL;
	const u8 *body = NULL;
	const u8 *challenge_data = NULL;
	size_t rbuflen;
	size_t body_len;
	size_t output_len;
	size_t challenge_len;
	size_t keylen = 0;
	size_t cypher_text_len = 0;
	u8 sbuf[255];
	EVP_CIPHER_CTX * ctx = NULL;
	const EVP_CIPHER *cipher;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
	    r = SC_ERROR_OUT_OF_MEMORY;
	    goto err;
	}

	sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Selected cipher for algorithm id: %02x\n", alg_id);

	cipher = get_cipher_for_algo(alg_id);
	if(!cipher) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Invalid cipher selector, none found for:  %02x\n", alg_id);
		r = SC_ERROR_INVALID_ARGUMENTS;
		goto err;
	}

	r = piv_get_key(card, alg_id, &key, &keylen);
	if (r) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Error getting General Auth key\n");
		goto err;
	}

	r = sc_lock(card);
	if (r != SC_SUCCESS) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "sc_lock failed\n");
		goto err;  
	}
	locked = 1;

	p = sbuf;
	*p++ = 0x7C;
	*p++ = 0x02;
	*p++ = 0x81;
	*p++ = 0x00;

	 
	r = piv_general_io(card, 0x87, alg_id, key_ref, sbuf, p - sbuf, &rbuf, &rbuflen);
	if (r < 0) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Error getting Challenge\n");
		goto err;
	}

	 
	output_len = r;

	 
	body = sc_asn1_find_tag(card->ctx, rbuf,
		r, 0x7C, &body_len);
	if (!body) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Invalid Challenge Data response of NULL\n");
		r =  SC_ERROR_INVALID_DATA;
		goto err;
	}

	 
	challenge_data = sc_asn1_find_tag(card->ctx, body,
		body_len, 0x81, &challenge_len);
	if (!challenge_data) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Invalid Challenge Data none found in TLV\n");
		r =  SC_ERROR_INVALID_DATA;
		goto err;
	}

	 
	 
	tmplen = challenge_len;

	 
	if (!EVP_EncryptInit(ctx, cipher, key, NULL)) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Encrypt fail\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	cypher_text = malloc(challenge_len);
	if (!cypher_text) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Could not allocate buffer for cipher text\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	EVP_CIPHER_CTX_set_padding(ctx,0);
	if (!EVP_EncryptUpdate(ctx, cypher_text, &outlen, challenge_data, challenge_len)) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Encrypt update fail\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	cypher_text_len += outlen;

	if (!EVP_EncryptFinal(ctx, cypher_text + cypher_text_len, &outlen)) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Final fail\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	cypher_text_len += outlen;

	 
	if (cypher_text_len != (size_t)tmplen) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Length test fail\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	output_buf = malloc(output_len);
	if(!output_buf) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Could not allocate output buffer: %s\n",
				strerror(errno));
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	p = output_buf;

	 
	tmplen = put_tag_and_len(0x82, cypher_text_len, NULL);

	tmplen = put_tag_and_len(0x7C, tmplen, &p);

	 
	tmplen += put_tag_and_len(0x82, cypher_text_len, &p);

	memcpy(p, cypher_text, cypher_text_len);
	p += cypher_text_len;
	tmplen += cypher_text_len;

	 
	if(output_len != (size_t)tmplen) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Allocated and computed lengths do not match! "
			 "Expected %"SC_FORMAT_LEN_SIZE_T"d, found: %d\n", output_len, tmplen);
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	r = piv_general_io(card, 0x87, alg_id, key_ref, output_buf, output_len, NULL, NULL);
	sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Got response  challenge\n");

err:
	if (ctx)
		EVP_CIPHER_CTX_free(ctx);

	if (locked)
		sc_unlock(card);

	if (key) {
		sc_mem_clear(key, keylen);
		free(key);
	}

	if (rbuf)
		free(rbuf);

	if (cypher_text)
		free(cypher_text);

	if (output_buf)
		free(output_buf);
#else
	sc_log(card->ctx, "OpenSSL Required");
	r = SC_ERROR_NOT_SUPPORTED;
#endif  

	LOG_FUNC_RETURN(card->ctx, r);
}
