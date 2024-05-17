static int ssl23_client_hello(SSL *s)
	{
	unsigned char *buf;
	unsigned char *p,*d;
	int i,ch_len;
	unsigned long l;
	int ssl2_compat;
	int version = 0, version_major, version_minor;
#ifndef OPENSSL_NO_COMP
	int j;
	SSL_COMP *comp;
#endif
	int ret;
	unsigned long mask, options = s->options;

	ssl2_compat = (options & SSL_OP_NO_SSLv2) ? 0 : 1;

	if (ssl2_compat && ssl23_no_ssl2_ciphers(s))
		ssl2_compat = 0;

	 
	mask =	SSL_OP_NO_TLSv1_1|SSL_OP_NO_TLSv1
#if !defined(OPENSSL_NO_SSL3)
		|SSL_OP_NO_SSLv3
#endif
#if !defined(OPENSSL_NO_SSL2)
		|(ssl2_compat?SSL_OP_NO_SSLv2:0)
#endif
		;
#if !defined(OPENSSL_NO_TLS1_2_CLIENT)
	version = TLS1_2_VERSION;

	if ((options & SSL_OP_NO_TLSv1_2) && (options & mask) != mask)
		version = TLS1_1_VERSION;
#else
	version = TLS1_1_VERSION;
#endif
	mask &= ~SSL_OP_NO_TLSv1_1;
	if ((options & SSL_OP_NO_TLSv1_1) && (options & mask) != mask)
		version = TLS1_VERSION;
	mask &= ~SSL_OP_NO_TLSv1;
#if !defined(OPENSSL_NO_SSL3)
	if ((options & SSL_OP_NO_TLSv1) && (options & mask) != mask)
		version = SSL3_VERSION;
	mask &= ~SSL_OP_NO_SSLv3;
#endif
#if !defined(OPENSSL_NO_SSL2)
	if ((options & SSL_OP_NO_SSLv3) && (options & mask) != mask)
		version = SSL2_VERSION;
#endif

#ifndef OPENSSL_NO_TLSEXT
	if (version != SSL2_VERSION)
		{
		 

		if (s->tlsext_hostname != NULL)
			ssl2_compat = 0;
		if (s->tlsext_status_type != -1)
			ssl2_compat = 0;
#ifdef TLSEXT_TYPE_opaque_prf_input
		if (s->ctx->tlsext_opaque_prf_input_callback != 0 || s->tlsext_opaque_prf_input != NULL)
			ssl2_compat = 0;
#endif
		}
#endif

	buf=(unsigned char *)s->init_buf->data;
	if (s->state == SSL23_ST_CW_CLNT_HELLO_A)
		{
#if 0
		 
		if (!ssl_get_new_session(s,0))
			{
			return(-1);
			}
#endif

		p=s->s3->client_random;
		if (ssl_fill_hello_random(s, 0, p, SSL3_RANDOM_SIZE) <= 0)
			return -1;

		if (version == TLS1_2_VERSION)
			{
			version_major = TLS1_2_VERSION_MAJOR;
			version_minor = TLS1_2_VERSION_MINOR;
			}
		else if (version == TLS1_1_VERSION)
			{
			version_major = TLS1_1_VERSION_MAJOR;
			version_minor = TLS1_1_VERSION_MINOR;
			}
		else if (version == TLS1_VERSION)
			{
			version_major = TLS1_VERSION_MAJOR;
			version_minor = TLS1_VERSION_MINOR;
			}
#ifdef OPENSSL_FIPS
		else if(FIPS_mode())
			{
			SSLerr(SSL_F_SSL23_CLIENT_HELLO,
					SSL_R_ONLY_TLS_ALLOWED_IN_FIPS_MODE);
			return -1;
			}
#endif
		else if (version == SSL3_VERSION)
			{
			version_major = SSL3_VERSION_MAJOR;
			version_minor = SSL3_VERSION_MINOR;
			}
		else if (version == SSL2_VERSION)
			{
			version_major = SSL2_VERSION_MAJOR;
			version_minor = SSL2_VERSION_MINOR;
			}
		else
			{
			SSLerr(SSL_F_SSL23_CLIENT_HELLO,SSL_R_NO_PROTOCOLS_AVAILABLE);
			return(-1);
			}

		s->client_version = version;

		if (ssl2_compat)
			{
			 

			 
			d = &(buf[2]);
			p = d + 9;  

			*(d++) = SSL2_MT_CLIENT_HELLO;
			*(d++) = version_major;
			*(d++) = version_minor;
			
			 
			i=ssl_cipher_list_to_bytes(s,SSL_get_ciphers(s),p,0);
			if (i == 0)
				{
				 
				SSLerr(SSL_F_SSL23_CLIENT_HELLO,SSL_R_NO_CIPHERS_AVAILABLE);
				return -1;
				}
			s2n(i,d);
			p+=i;
			
			 
#if 0
			s->session->session_id_length=0;
#endif
			s2n(0,d);

			if (s->options & SSL_OP_NETSCAPE_CHALLENGE_BUG)
				ch_len=SSL2_CHALLENGE_LENGTH;
			else
				ch_len=SSL2_MAX_CHALLENGE_LENGTH;

			 
			 
			if (SSL3_RANDOM_SIZE < ch_len)
				i=SSL3_RANDOM_SIZE;
			else
				i=ch_len;
			s2n(i,d);
			memset(&(s->s3->client_random[0]),0,SSL3_RANDOM_SIZE);
			if (RAND_pseudo_bytes(&(s->s3->client_random[SSL3_RANDOM_SIZE-i]),i) <= 0)
				return -1;

			memcpy(p,&(s->s3->client_random[SSL3_RANDOM_SIZE-i]),i);
			p+=i;

			i= p- &(buf[2]);
			buf[0]=((i>>8)&0xff)|0x80;
			buf[1]=(i&0xff);

			 
			s->init_num=i+2;
			s->init_off=0;

			ssl3_finish_mac(s,&(buf[2]),i);
			}
		else
			{
			 

			 
			d = p = &(buf[9]);
			
			*(p++) = version_major;
			*(p++) = version_minor;

			 
			memcpy(p, s->s3->client_random, SSL3_RANDOM_SIZE);
			p += SSL3_RANDOM_SIZE;

			 
			*(p++) = 0;

			 
			i=ssl_cipher_list_to_bytes(s,SSL_get_ciphers(s),&(p[2]),ssl3_put_cipher_by_char);
			if (i == 0)
				{
				SSLerr(SSL_F_SSL23_CLIENT_HELLO,SSL_R_NO_CIPHERS_AVAILABLE);
				return -1;
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
				SSLerr(SSL_F_SSL23_CLIENT_HELLO,SSL_R_CLIENTHELLO_TLSEXT);
				return -1;
				}
			if ((p = ssl_add_clienthello_tlsext(s, p, buf+SSL3_RT_MAX_PLAIN_LENGTH)) == NULL)
				{
				SSLerr(SSL_F_SSL23_CLIENT_HELLO,ERR_R_INTERNAL_ERROR);
				return -1;
				}
#endif
			
			l = p-d;

			 
			d=&(buf[5]);
			*(d++)=SSL3_MT_CLIENT_HELLO;
			l2n3(l,d);

			l += 4;

			if (l > SSL3_RT_MAX_PLAIN_LENGTH)
				{
				SSLerr(SSL_F_SSL23_CLIENT_HELLO,ERR_R_INTERNAL_ERROR);
				return -1;
				}
			
			 
			d=buf;
			*(d++) = SSL3_RT_HANDSHAKE;
			*(d++) = version_major;
			 
			if (TLS1_get_client_version(s) > TLS1_VERSION)
				*(d++) = 1;
			else
				*(d++) = version_minor;
			s2n((int)l,d);

			 
			s->init_num=p-buf;
			s->init_off=0;

			ssl3_finish_mac(s,&(buf[5]), s->init_num - 5);
			}

		s->state=SSL23_ST_CW_CLNT_HELLO_B;
		s->init_off=0;
		}

	 
	ret = ssl23_write_bytes(s);

	if ((ret >= 2) && s->msg_callback)
		{
		 

		if (ssl2_compat)
			s->msg_callback(1, SSL2_VERSION, 0, s->init_buf->data+2, ret-2, s, s->msg_callback_arg);
		else
			s->msg_callback(1, version, SSL3_RT_HANDSHAKE, s->init_buf->data+5, ret-5, s, s->msg_callback_arg);
		}

	return ret;
	}
