void ssl_set_client_disabled(SSL *s)
	{
	CERT *c = s->cert;
	c->mask_a = 0;
	c->mask_k = 0;
	 
	if (!SSL_CLIENT_USE_TLS1_2_CIPHERS(s))
		c->mask_ssl = SSL_TLSV1_2;
	else
		c->mask_ssl = 0;
	ssl_set_sig_mask(&c->mask_a, s, SSL_SECOP_SIGALG_MASK);
	 
	if (c->mask_a & SSL_aRSA)
		c->mask_k |= SSL_kDHr|SSL_kECDHr;
	if (c->mask_a & SSL_aDSS)
		c->mask_k |= SSL_kDHd;
	if (c->mask_a & SSL_aECDSA)
		c->mask_k |= SSL_kECDHe;
#ifndef OPENSSL_NO_KRB5
	if (!kssl_tgt_is_available(s->kssl_ctx))
		{
		c->mask_a |= SSL_aKRB5;
		c->mask_k |= SSL_kKRB5;
		}
#endif
#ifndef OPENSSL_NO_PSK
	 
	if (!s->psk_client_callback)
		{
		c->mask_a |= SSL_aPSK;
		c->mask_k |= SSL_kPSK;
		}
#endif  
	c->valid = 1;
	}
