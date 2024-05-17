int ssl3_send_newsession_ticket(SSL *s)
	{
	if (s->state == SSL3_ST_SW_SESSION_TICKET_A)
		{
		unsigned char *p, *senc, *macstart;
		const unsigned char *const_p;
		int len, slen_full, slen;
		SSL_SESSION *sess;
		unsigned int hlen;
		EVP_CIPHER_CTX ctx;
		HMAC_CTX hctx;
		SSL_CTX *tctx = s->initial_ctx;
		unsigned char iv[EVP_MAX_IV_LENGTH];
		unsigned char key_name[16];

		 
		slen_full = i2d_SSL_SESSION(s->session, NULL);
		 
		if (slen_full > 0xFF00)
			return -1;
		senc = OPENSSL_malloc(slen_full);
		if (!senc)
			return -1;
		p = senc;
		i2d_SSL_SESSION(s->session, &p);

		 
		const_p = senc;
		sess = d2i_SSL_SESSION(NULL, &const_p, slen_full);
		if (sess == NULL)
			{
			OPENSSL_free(senc);
			return -1;
			}
		sess->session_id_length = 0;  

		slen = i2d_SSL_SESSION(sess, NULL);
		if (slen > slen_full)  
			{
			OPENSSL_free(senc);
			return -1;
			}
		p = senc;
		i2d_SSL_SESSION(sess, &p);
		SSL_SESSION_free(sess);

		 
		if (!BUF_MEM_grow(s->init_buf,
			SSL_HM_HEADER_LENGTH(s) + 22 + EVP_MAX_IV_LENGTH +
			EVP_MAX_BLOCK_LENGTH + EVP_MAX_MD_SIZE + slen))
			return -1;
		p = ssl_handshake_start(s);
		EVP_CIPHER_CTX_init(&ctx);
		HMAC_CTX_init(&hctx);
		 
		if (tctx->tlsext_ticket_key_cb)
			{
			if (tctx->tlsext_ticket_key_cb(s, key_name, iv, &ctx,
							 &hctx, 1) < 0)
				{
				OPENSSL_free(senc);
				return -1;
				}
			}
		else
			{
			RAND_pseudo_bytes(iv, 16);
			EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL,
					tctx->tlsext_tick_aes_key, iv);
			HMAC_Init_ex(&hctx, tctx->tlsext_tick_hmac_key, 16,
					tlsext_tick_md(), NULL);
			memcpy(key_name, tctx->tlsext_tick_key_name, 16);
			}

		 
		l2n(s->hit ? 0 : s->session->timeout, p);

		 
		p += 2;
		 
		macstart = p;
		memcpy(p, key_name, 16);
		p += 16;
		 
		memcpy(p, iv, EVP_CIPHER_CTX_iv_length(&ctx));
		p += EVP_CIPHER_CTX_iv_length(&ctx);
		 
		EVP_EncryptUpdate(&ctx, p, &len, senc, slen);
		p += len;
		EVP_EncryptFinal(&ctx, p, &len);
		p += len;
		EVP_CIPHER_CTX_cleanup(&ctx);

		HMAC_Update(&hctx, macstart, p - macstart);
		HMAC_Final(&hctx, p, &hlen);
		HMAC_CTX_cleanup(&hctx);

		p += hlen;
		 
		 
		len = p - ssl_handshake_start(s);
		ssl_set_handshake_header(s, SSL3_MT_NEWSESSION_TICKET, len);
		 
		p = ssl_handshake_start(s) + 4;
		s2n(len - 6, p);
		s->state=SSL3_ST_SW_SESSION_TICKET_B;
		OPENSSL_free(senc);
		}

	 
	return ssl_do_write(s);
	}
