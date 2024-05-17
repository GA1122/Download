int ssl3_client_hello(SSL *s)
	{
	unsigned char *buf;
	unsigned char *p,*d;
	int i;
	unsigned long l;
#ifndef OPENSSL_NO_COMP
	int j;
	SSL_COMP *comp;
#endif

	buf=(unsigned char *)s->init_buf->data;
	if (s->state == SSL3_ST_CW_CLNT_HELLO_A)
		{
		SSL_SESSION *sess = s->session;
		if ((sess == NULL) ||
			(sess->ssl_version != s->version) ||
#ifdef OPENSSL_NO_TLSEXT
			!sess->session_id_length ||
#else
			(!sess->session_id_length && !sess->tlsext_tick) ||
#endif
			(sess->not_resumable))
			{
			if (!ssl_get_new_session(s,0))
				goto err;
			}
		 

		p=s->s3->client_random;

		if (ssl_fill_hello_random(s, 0, p, SSL3_RANDOM_SIZE) <= 0)
			goto err;

		 
		d=p= &(buf[4]);

		 
#if 0
		*(p++)=s->version>>8;
		*(p++)=s->version&0xff;
		s->client_version=s->version;
#else
		*(p++)=s->client_version>>8;
		*(p++)=s->client_version&0xff;
#endif

		 
		memcpy(p,s->s3->client_random,SSL3_RANDOM_SIZE);
		p+=SSL3_RANDOM_SIZE;

		 
		if (s->new_session)
			i=0;
		else
			i=s->session->session_id_length;
		*(p++)=i;
		if (i != 0)
			{
			if (i > (int)sizeof(s->session->session_id))
				{
				SSLerr(SSL_F_SSL3_CLIENT_HELLO, ERR_R_INTERNAL_ERROR);
				goto err;
				}
			memcpy(p,s->session->session_id,i);
			p+=i;
			}
		
		 
		i=ssl_cipher_list_to_bytes(s,SSL_get_ciphers(s),&(p[2]),0);
		if (i == 0)
			{
			SSLerr(SSL_F_SSL3_CLIENT_HELLO,SSL_R_NO_CIPHERS_AVAILABLE);
			goto err;
			}
#ifdef OPENSSL_MAX_TLS1_2_CIPHER_LENGTH
			 
			if (TLS1_get_version(s) >= TLS1_2_VERSION
				&& i > OPENSSL_MAX_TLS1_2_CIPHER_LENGTH)
				i = OPENSSL_MAX_TLS1_2_CIPHER_LENGTH & ~1;
#endif
		s2n(i,p);
		p+=i;

		 
#ifdef OPENSSL_NO_COMP
		*(p++)=1;
#else

		if ((s->options & SSL_OP_NO_COMPRESSION)
					|| !s->ctx->comp_methods)
			j=0;
		else
			j=sk_SSL_COMP_num(s->ctx->comp_methods);
		*(p++)=1+j;
		for (i=0; i<j; i++)
			{
			comp=sk_SSL_COMP_value(s->ctx->comp_methods,i);
			*(p++)=comp->id;
			}
#endif
		*(p++)=0;  

#ifndef OPENSSL_NO_TLSEXT
		 
		if (ssl_prepare_clienthello_tlsext(s) <= 0)
			{
			SSLerr(SSL_F_SSL3_CLIENT_HELLO,SSL_R_CLIENTHELLO_TLSEXT);
			goto err;
			}
		if ((p = ssl_add_clienthello_tlsext(s, p, buf+SSL3_RT_MAX_PLAIN_LENGTH)) == NULL)
			{
			SSLerr(SSL_F_SSL3_CLIENT_HELLO,ERR_R_INTERNAL_ERROR);
			goto err;
			}
#endif
		
		l=(p-d);
		d=buf;
		*(d++)=SSL3_MT_CLIENT_HELLO;
		l2n3(l,d);

		s->state=SSL3_ST_CW_CLNT_HELLO_B;
		 
		s->init_num=p-buf;
		s->init_off=0;
		}

	 
	return(ssl3_do_write(s,SSL3_RT_HANDSHAKE));
err:
	return(-1);
	}
