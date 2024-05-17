static int piv_general_mutual_authenticate(sc_card_t *card,
	unsigned int key_ref, unsigned int alg_id)
{
	int r;
#ifdef ENABLE_OPENSSL
	int N;
	int locked = 0;
	u8  *rbuf = NULL;
	size_t rbuflen;
	u8 *nonce = NULL;
	size_t nonce_len;
	u8 *p;
	u8 *key = NULL;
	size_t keylen;
	u8 *plain_text = NULL;
	size_t plain_text_len = 0;
	u8 *tmp;
	size_t tmplen, tmplen2;
	u8 *built = NULL;
	size_t built_len;
	const u8 *body = NULL;
	size_t body_len;
	const u8 *witness_data = NULL;
	size_t witness_len;
	const u8 *challenge_response = NULL;
	size_t challenge_response_len;
	u8 *decrypted_reponse = NULL;
	size_t decrypted_reponse_len;
	EVP_CIPHER_CTX * ctx = NULL;

	u8 sbuf[255];
	const EVP_CIPHER *cipher;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		r = SC_ERROR_OUT_OF_MEMORY;
		goto err;
	}

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
	*p++ = 0x80;
	*p++ = 0x00;

	 
	r = piv_general_io(card, 0x87, alg_id, key_ref, sbuf, p - sbuf, &rbuf, &rbuflen);

 	if (r < 0) goto err;

	 
	body = sc_asn1_find_tag(card->ctx, rbuf,
		r, 0x7C, &body_len);
	if (!body) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Invalid Witness Data response of NULL\n");
		r =  SC_ERROR_INVALID_DATA;
		goto err;
	}

	 
	witness_data = sc_asn1_find_tag(card->ctx, body,
		body_len, 0x80, &witness_len);
	if (!witness_len) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Invalid Challenge Data none found in TLV\n");
		r =  SC_ERROR_INVALID_DATA;
		goto err;
	}

	 
	plain_text = malloc(witness_len);
	if (!plain_text) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Could not allocate buffer for plain text\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	 
	if (!EVP_DecryptInit(ctx, cipher, key, NULL)) {
		 
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	EVP_CIPHER_CTX_set_padding(ctx,0);

	p = plain_text;
	if (!EVP_DecryptUpdate(ctx, p, &N, witness_data, witness_len)) {
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	plain_text_len = tmplen = N;
	p += tmplen;

	if(!EVP_DecryptFinal(ctx, p, &N)) {
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	tmplen = N;
	plain_text_len += tmplen;

	if (plain_text_len != witness_len) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,
			 "Encrypted and decrypted lengths do not match: %"SC_FORMAT_LEN_SIZE_T"u:%"SC_FORMAT_LEN_SIZE_T"u\n",
			 witness_len, plain_text_len);
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	 
	nonce = malloc(witness_len);
	if(!nonce) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,
			 "OOM allocating nonce (%"SC_FORMAT_LEN_SIZE_T"u : %"SC_FORMAT_LEN_SIZE_T"u)\n",
			 witness_len, plain_text_len);
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	nonce_len = witness_len;

	r = RAND_bytes(nonce, witness_len);
	if(!r) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,
			 "Generating random for nonce (%"SC_FORMAT_LEN_SIZE_T"u : %"SC_FORMAT_LEN_SIZE_T"u)\n",
			 witness_len, plain_text_len);
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	 
	tmplen = put_tag_and_len(0x81, witness_len, NULL);

	 
	tmplen += put_tag_and_len(0x80, witness_len, NULL);
	tmplen2 = tmplen;

	 
	tmplen = put_tag_and_len(0x7C, tmplen, NULL);

	built_len = tmplen;

	 
	p = built = malloc(built_len);
	if(!built) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "OOM Building witness response and challenge\n");
		r = SC_ERROR_INTERNAL;
		goto err;
	}

	p = built;

	 
	put_tag_and_len(0x7C, tmplen2, &p);

	 
	put_tag_and_len(0x80, witness_len, &p);
	memcpy(p, plain_text, witness_len);
	p += witness_len;

	 
	put_tag_and_len(0x81, witness_len, &p);
	memcpy(p, nonce, witness_len);

	 
	free(rbuf);
	rbuf = NULL;

	 
	r = piv_general_io(card, 0x87, alg_id, key_ref, built,built_len, &rbuf, &rbuflen);
 	if (r < 0) goto err;

	 
	body = sc_asn1_find_tag(card->ctx, rbuf,
		r, 0x7C, &body_len);
	if(!body) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Could not find outer tag 0x7C in response");
		r =  SC_ERROR_INVALID_DATA;
		goto err;
	}

	 
	challenge_response = sc_asn1_find_tag(card->ctx, body,
		body_len, 0x82, &challenge_response_len);
	if(!challenge_response) {
		challenge_response = sc_asn1_find_tag(card->ctx, body,
				body_len, 0x80, &challenge_response_len);
		if(!challenge_response) {
			sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Could not find tag 0x82 or 0x80 in response");
			r =  SC_ERROR_INVALID_DATA;
			goto err;
		}
	}

	 
	decrypted_reponse = malloc(challenge_response_len);
	if(!decrypted_reponse) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "OOM Allocating decryption buffer");
		r =  SC_ERROR_INVALID_DATA;
		goto err;
	}

	EVP_CIPHER_CTX_cleanup(ctx);

	if (!EVP_DecryptInit(ctx, cipher, key, NULL)) {
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	EVP_CIPHER_CTX_set_padding(ctx,0);

	tmp = decrypted_reponse;
	if (!EVP_DecryptUpdate(ctx, tmp, &N, challenge_response, challenge_response_len)) {
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	decrypted_reponse_len = tmplen = N;
	tmp += tmplen;

	if(!EVP_DecryptFinal(ctx, tmp, &N)) {
		r = SC_ERROR_INTERNAL;
		goto err;
	}
	tmplen = N;
	decrypted_reponse_len += tmplen;

	if (decrypted_reponse_len != nonce_len || memcmp(nonce, decrypted_reponse, nonce_len) != 0) {
		sc_log(card->ctx,
		       "mutual authentication failed, card returned wrong value %"SC_FORMAT_LEN_SIZE_T"u:%"SC_FORMAT_LEN_SIZE_T"u",
		       decrypted_reponse_len, nonce_len);
		r = SC_ERROR_DECRYPT_FAILED;
		goto err;
	}
	r = SC_SUCCESS;

err:
	if (ctx)
		EVP_CIPHER_CTX_free(ctx);
	if (locked)
		sc_unlock(card);
	if (rbuf)
		free(rbuf);
	if (decrypted_reponse)
		free(decrypted_reponse);
	if (built)
		free(built);
	if (plain_text)
		free(plain_text);
	if (nonce)
		free(nonce);
	if (key)
		free(key);

#else
	sc_log(card->ctx, "OpenSSL Required");
	r = SC_ERROR_NOT_SUPPORTED;
#endif  

	LOG_FUNC_RETURN(card->ctx, r);
}
