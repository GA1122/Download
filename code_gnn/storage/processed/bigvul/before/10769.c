int SSL_clear(SSL *s)
	{

	if (s->method == NULL)
		{
		SSLerr(SSL_F_SSL_CLEAR,SSL_R_NO_METHOD_SPECIFIED);
		return(0);
		}

	if (ssl_clear_bad_session(s))
		{
		SSL_SESSION_free(s->session);
		s->session=NULL;
		}

	s->error=0;
	s->hit=0;
	s->shutdown=0;

#if 0  
	 
	if (s->renegotiate) return(1);
#else
	if (s->renegotiate)
		{
		SSLerr(SSL_F_SSL_CLEAR,ERR_R_INTERNAL_ERROR);
		return 0;
		}
#endif

	s->type=0;

	s->state=SSL_ST_BEFORE|((s->server)?SSL_ST_ACCEPT:SSL_ST_CONNECT);

	s->version=s->method->version;
	s->client_version=s->version;
	s->rwstate=SSL_NOTHING;
	s->rstate=SSL_ST_READ_HEADER;
#if 0
	s->read_ahead=s->ctx->read_ahead;
#endif

	if (s->init_buf != NULL)
		{
		BUF_MEM_free(s->init_buf);
		s->init_buf=NULL;
		}

	ssl_clear_cipher_ctx(s);
	ssl_clear_hash_ctx(&s->read_hash);
	ssl_clear_hash_ctx(&s->write_hash);

	s->first_packet=0;

#if 1
	 
	if (!s->in_handshake && (s->session == NULL) && (s->method != s->ctx->method))
		{
		s->method->ssl_free(s);
		s->method=s->ctx->method;
		if (!s->method->ssl_new(s))
			return(0);
		}
	else
#endif
		s->method->ssl_clear(s);
	return(1);
	}
