int ssl_cipher_disabled(SSL *s, const SSL_CIPHER *c, int op)
	{
	CERT *ct = s->cert;
	if (c->algorithm_ssl & ct->mask_ssl || c->algorithm_mkey & ct->mask_k || c->algorithm_auth & ct->mask_a)
		return 1;
	return !ssl_security(s, op, c->strength_bits, 0, (void *)c);
	}
