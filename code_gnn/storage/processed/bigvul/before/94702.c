int ssl3_read_bytes(SSL *s, int type, unsigned char *buf, int len, int peek)
	{
	int al,i,j,ret;
	unsigned int n;
	SSL3_RECORD *rr;
	void (*cb)(const SSL *ssl,int type2,int val)=NULL;

	if (s->s3->rbuf.buf == NULL)  
		if (!ssl3_setup_read_buffer(s))
			return(-1);

	if ((type && (type != SSL3_RT_APPLICATION_DATA) && (type != SSL3_RT_HANDSHAKE)) ||
	    (peek && (type != SSL3_RT_APPLICATION_DATA)))
		{
		SSLerr(SSL_F_SSL3_READ_BYTES, ERR_R_INTERNAL_ERROR);
		return -1;
		}

	if ((type == SSL3_RT_HANDSHAKE) && (s->s3->handshake_fragment_len > 0))
		 
		{
		unsigned char *src = s->s3->handshake_fragment;
		unsigned char *dst = buf;
		unsigned int k;

		 
		n = 0;
		while ((len > 0) && (s->s3->handshake_fragment_len > 0))
			{
			*dst++ = *src++;
			len--; s->s3->handshake_fragment_len--;
			n++;
			}
		 
		for (k = 0; k < s->s3->handshake_fragment_len; k++)
			s->s3->handshake_fragment[k] = *src++;
		return n;
	}

	 

	if (!s->in_handshake && SSL_in_init(s))
		{
		 
		i=s->handshake_func(s);
		if (i < 0) return(i);
		if (i == 0)
			{
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_SSL_HANDSHAKE_FAILURE);
			return(-1);
			}
		}
start:
	s->rwstate=SSL_NOTHING;

	 
	rr = &(s->s3->rrec);

	 
	if ((rr->length == 0) || (s->rstate == SSL_ST_READ_BODY))
		{
		ret=ssl3_get_record(s);
		if (ret <= 0) return(ret);
		}

	 

	if (s->s3->change_cipher_spec  
		&& (rr->type != SSL3_RT_HANDSHAKE))
		{
		al=SSL_AD_UNEXPECTED_MESSAGE;
		SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_DATA_BETWEEN_CCS_AND_FINISHED);
		goto f_err;
		}

	 
	if (s->shutdown & SSL_RECEIVED_SHUTDOWN)
		{
		rr->length=0;
		s->rwstate=SSL_NOTHING;
		return(0);
		}


	if (type == rr->type)  
		{
		 
		if (SSL_in_init(s) && (type == SSL3_RT_APPLICATION_DATA) &&
			(s->enc_read_ctx == NULL))
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_APP_DATA_IN_HANDSHAKE);
			goto f_err;
			}

		if (len <= 0) return(len);

		if ((unsigned int)len > rr->length)
			n = rr->length;
		else
			n = (unsigned int)len;

		memcpy(buf,&(rr->data[rr->off]),n);
		if (!peek)
			{
			rr->length-=n;
			rr->off+=n;
			if (rr->length == 0)
				{
				s->rstate=SSL_ST_READ_HEADER;
				rr->off=0;
				if (s->mode & SSL_MODE_RELEASE_BUFFERS && s->s3->rbuf.left == 0)
					ssl3_release_read_buffer(s);
				}
			}
		return(n);
		}


	 

	 
		{
		unsigned int dest_maxlen = 0;
		unsigned char *dest = NULL;
		unsigned int *dest_len = NULL;

		if (rr->type == SSL3_RT_HANDSHAKE)
			{
			dest_maxlen = sizeof s->s3->handshake_fragment;
			dest = s->s3->handshake_fragment;
			dest_len = &s->s3->handshake_fragment_len;
			}
		else if (rr->type == SSL3_RT_ALERT)
			{
			dest_maxlen = sizeof s->s3->alert_fragment;
			dest = s->s3->alert_fragment;
			dest_len = &s->s3->alert_fragment_len;
			}
#ifndef OPENSSL_NO_HEARTBEATS
		else if (rr->type == TLS1_RT_HEARTBEAT)
			{
			tls1_process_heartbeat(s);

			 
			rr->length = 0;
			s->rwstate=SSL_READING;
			BIO_clear_retry_flags(SSL_get_rbio(s));
			BIO_set_retry_read(SSL_get_rbio(s));
			return(-1);
			}
#endif

		if (dest_maxlen > 0)
			{
			n = dest_maxlen - *dest_len;  
			if (rr->length < n)
				n = rr->length;  

			 
			while (n-- > 0)
				{
				dest[(*dest_len)++] = rr->data[rr->off++];
				rr->length--;
				}

			if (*dest_len < dest_maxlen)
				goto start;  
			}
		}

	 

	 
	if ((!s->server) &&
		(s->s3->handshake_fragment_len >= 4) &&
		(s->s3->handshake_fragment[0] == SSL3_MT_HELLO_REQUEST) &&
		(s->session != NULL) && (s->session->cipher != NULL))
		{
		s->s3->handshake_fragment_len = 0;

		if ((s->s3->handshake_fragment[1] != 0) ||
			(s->s3->handshake_fragment[2] != 0) ||
			(s->s3->handshake_fragment[3] != 0))
			{
			al=SSL_AD_DECODE_ERROR;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_BAD_HELLO_REQUEST);
			goto f_err;
			}

		if (s->msg_callback)
			s->msg_callback(0, s->version, SSL3_RT_HANDSHAKE, s->s3->handshake_fragment, 4, s, s->msg_callback_arg);

		if (SSL_is_init_finished(s) &&
			!(s->s3->flags & SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS) &&
			!s->s3->renegotiate)
			{
			ssl3_renegotiate(s);
			if (ssl3_renegotiate_check(s))
				{
				i=s->handshake_func(s);
				if (i < 0) return(i);
				if (i == 0)
					{
					SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_SSL_HANDSHAKE_FAILURE);
					return(-1);
					}

				if (!(s->mode & SSL_MODE_AUTO_RETRY))
					{
					if (s->s3->rbuf.left == 0)  
						{
						BIO *bio;
						 
						s->rwstate=SSL_READING;
						bio=SSL_get_rbio(s);
						BIO_clear_retry_flags(bio);
						BIO_set_retry_read(bio);
						return(-1);
						}
					}
				}
			}
		 
		goto start;
		}
	 
	if (s->server &&
		SSL_is_init_finished(s) &&
    		!s->s3->send_connection_binding &&
		(s->version > SSL3_VERSION) &&
		(s->s3->handshake_fragment_len >= 4) &&
		(s->s3->handshake_fragment[0] == SSL3_MT_CLIENT_HELLO) &&
		(s->session != NULL) && (s->session->cipher != NULL) &&
		!(s->ctx->options & SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION))
		
		{
		 
		rr->length = 0;
		ssl3_send_alert(s,SSL3_AL_WARNING, SSL_AD_NO_RENEGOTIATION);
		goto start;
		}
	if (s->s3->alert_fragment_len >= 2)
		{
		int alert_level = s->s3->alert_fragment[0];
		int alert_descr = s->s3->alert_fragment[1];

		s->s3->alert_fragment_len = 0;

		if (s->msg_callback)
			s->msg_callback(0, s->version, SSL3_RT_ALERT, s->s3->alert_fragment, 2, s, s->msg_callback_arg);

		if (s->info_callback != NULL)
			cb=s->info_callback;
		else if (s->ctx->info_callback != NULL)
			cb=s->ctx->info_callback;

		if (cb != NULL)
			{
			j = (alert_level << 8) | alert_descr;
			cb(s, SSL_CB_READ_ALERT, j);
			}

		if (alert_level == 1)  
			{
			s->s3->warn_alert = alert_descr;
			if (alert_descr == SSL_AD_CLOSE_NOTIFY)
				{
				s->shutdown |= SSL_RECEIVED_SHUTDOWN;
				return(0);
				}
			 
			else if (alert_descr == SSL_AD_NO_RENEGOTIATION)
				{
				al = SSL_AD_HANDSHAKE_FAILURE;
				SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_NO_RENEGOTIATION);
				goto f_err;
				}
#ifdef SSL_AD_MISSING_SRP_USERNAME
			else if (alert_descr == SSL_AD_MISSING_SRP_USERNAME)
				return(0);
#endif
			}
		else if (alert_level == 2)  
			{
			char tmp[16];

			s->rwstate=SSL_NOTHING;
			s->s3->fatal_alert = alert_descr;
			SSLerr(SSL_F_SSL3_READ_BYTES, SSL_AD_REASON_OFFSET + alert_descr);
			BIO_snprintf(tmp,sizeof tmp,"%d",alert_descr);
			ERR_add_error_data(2,"SSL alert number ",tmp);
			s->shutdown|=SSL_RECEIVED_SHUTDOWN;
			SSL_CTX_remove_session(s->ctx,s->session);
			return(0);
			}
		else
			{
			al=SSL_AD_ILLEGAL_PARAMETER;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_UNKNOWN_ALERT_TYPE);
			goto f_err;
			}

		goto start;
		}

	if (s->shutdown & SSL_SENT_SHUTDOWN)  
		{
		s->rwstate=SSL_NOTHING;
		rr->length=0;
		return(0);
		}

	if (rr->type == SSL3_RT_CHANGE_CIPHER_SPEC)
		{
		 
		if (	(rr->length != 1) || (rr->off != 0) ||
			(rr->data[0] != SSL3_MT_CCS))
			{
			al=SSL_AD_ILLEGAL_PARAMETER;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_BAD_CHANGE_CIPHER_SPEC);
			goto f_err;
			}

		 
		if (s->s3->tmp.new_cipher == NULL)
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_CCS_RECEIVED_EARLY);
			goto f_err;
			}

		if (!(s->s3->flags & SSL3_FLAGS_CCS_OK))
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_CCS_RECEIVED_EARLY);
			goto f_err;
			}

		s->s3->flags &= ~SSL3_FLAGS_CCS_OK;

		rr->length=0;

		if (s->msg_callback)
			s->msg_callback(0, s->version, SSL3_RT_CHANGE_CIPHER_SPEC, rr->data, 1, s, s->msg_callback_arg);

		s->s3->change_cipher_spec=1;
		if (!ssl3_do_change_cipher_spec(s))
			goto err;
		else
			goto start;
		}

	 
	if ((s->s3->handshake_fragment_len >= 4) &&	!s->in_handshake)
		{
		if (((s->state&SSL_ST_MASK) == SSL_ST_OK) &&
			!(s->s3->flags & SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS))
			{
#if 0  
			s->state=SSL_ST_BEFORE|(s->server)
				?SSL_ST_ACCEPT
				:SSL_ST_CONNECT;
#else
			s->state = s->server ? SSL_ST_ACCEPT : SSL_ST_CONNECT;
#endif
			s->renegotiate=1;
			s->new_session=1;
			}
		i=s->handshake_func(s);
		if (i < 0) return(i);
		if (i == 0)
			{
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_SSL_HANDSHAKE_FAILURE);
			return(-1);
			}

		if (!(s->mode & SSL_MODE_AUTO_RETRY))
			{
			if (s->s3->rbuf.left == 0)  
				{
				BIO *bio;
				 
				s->rwstate=SSL_READING;
				bio=SSL_get_rbio(s);
				BIO_clear_retry_flags(bio);
				BIO_set_retry_read(bio);
				return(-1);
				}
			}
		goto start;
		}

	switch (rr->type)
		{
	default:
#ifndef OPENSSL_NO_TLS
		 
		if (s->version >= TLS1_VERSION && s->version <= TLS1_1_VERSION)
			{
			rr->length = 0;
			goto start;
			}
#endif
		al=SSL_AD_UNEXPECTED_MESSAGE;
		SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_UNEXPECTED_RECORD);
		goto f_err;
	case SSL3_RT_CHANGE_CIPHER_SPEC:
	case SSL3_RT_ALERT:
	case SSL3_RT_HANDSHAKE:
		 
		al=SSL_AD_UNEXPECTED_MESSAGE;
		SSLerr(SSL_F_SSL3_READ_BYTES,ERR_R_INTERNAL_ERROR);
		goto f_err;
	case SSL3_RT_APPLICATION_DATA:
		 
		if (s->s3->in_read_app_data &&
			(s->s3->total_renegotiations != 0) &&
			((
				(s->state & SSL_ST_CONNECT) &&
				(s->state >= SSL3_ST_CW_CLNT_HELLO_A) &&
				(s->state <= SSL3_ST_CR_SRVR_HELLO_A)
				) || (
					(s->state & SSL_ST_ACCEPT) &&
					(s->state <= SSL3_ST_SW_HELLO_REQ_A) &&
					(s->state >= SSL3_ST_SR_CLNT_HELLO_A)
					)
				))
			{
			s->s3->in_read_app_data=2;
			return(-1);
			}
		else
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_SSL3_READ_BYTES,SSL_R_UNEXPECTED_RECORD);
			goto f_err;
			}
		}
	 

f_err:
	ssl3_send_alert(s,SSL3_AL_FATAL,al);
err:
	return(-1);
	}
