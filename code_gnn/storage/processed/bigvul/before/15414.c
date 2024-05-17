int tls1_generate_master_secret(SSL *s, unsigned char *out, unsigned char *p,
	     int len)
	{
	unsigned char buff[SSL_MAX_MASTER_KEY_LENGTH];
	const void *co = NULL, *so = NULL;
	int col = 0, sol = 0;


#ifdef KSSL_DEBUG
	printf ("tls1_generate_master_secret(%p,%p, %p, %d)\n", s,out, p,len);
#endif	 

#ifdef TLSEXT_TYPE_opaque_prf_input
	if (s->s3->client_opaque_prf_input != NULL && s->s3->server_opaque_prf_input != NULL &&
	    s->s3->client_opaque_prf_input_len > 0 &&
	    s->s3->client_opaque_prf_input_len == s->s3->server_opaque_prf_input_len)
		{
		co = s->s3->client_opaque_prf_input;
		col = s->s3->server_opaque_prf_input_len;
		so = s->s3->server_opaque_prf_input;
		sol = s->s3->client_opaque_prf_input_len;  
		}
#endif

	tls1_PRF(ssl_get_algorithm2(s),
		TLS_MD_MASTER_SECRET_CONST,TLS_MD_MASTER_SECRET_CONST_SIZE,
		s->s3->client_random,SSL3_RANDOM_SIZE,
		co, col,
		s->s3->server_random,SSL3_RANDOM_SIZE,
		so, sol,
		p,len,
		s->session->master_key,buff,sizeof buff);
#ifdef SSL_DEBUG
	fprintf(stderr, "Premaster Secret:\n");
	BIO_dump_fp(stderr, (char *)p, len);
	fprintf(stderr, "Client Random:\n");
	BIO_dump_fp(stderr, (char *)s->s3->client_random, SSL3_RANDOM_SIZE);
	fprintf(stderr, "Server Random:\n");
	BIO_dump_fp(stderr, (char *)s->s3->server_random, SSL3_RANDOM_SIZE);
	fprintf(stderr, "Master Secret:\n");
	BIO_dump_fp(stderr, (char *)s->session->master_key, SSL3_MASTER_SECRET_SIZE);
#endif

#ifdef KSSL_DEBUG
	printf ("tls1_generate_master_secret() complete\n");
#endif	 
	return(SSL3_MASTER_SECRET_SIZE);
	}
