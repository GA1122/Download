int do_dtls1_write(SSL *s, int type, const unsigned char *buf, unsigned int len, int create_empty_fragment)
	{
	unsigned char *p,*pseq;
	int i,mac_size,clear=0;
	int prefix_len = 0;
	int eivlen;
	SSL3_RECORD *wr;
	SSL3_BUFFER *wb;
	SSL_SESSION *sess;

	 
	if (s->s3->wbuf.left != 0)
		{
		OPENSSL_assert(0);  
		return(ssl3_write_pending(s,type,buf,len));
		}

	 
	if (s->s3->alert_dispatch)
		{
		i=s->method->ssl_dispatch_alert(s);
		if (i <= 0)
			return(i);
		 
		}

	if (len == 0 && !create_empty_fragment)
		return 0;

	wr= &(s->s3->wrec);
	wb= &(s->s3->wbuf);
	sess=s->session;

	if (	(sess == NULL) ||
		(s->enc_write_ctx == NULL) ||
		(EVP_MD_CTX_md(s->write_hash) == NULL))
		clear=1;

	if (clear)
		mac_size=0;
	else
		{
		mac_size=EVP_MD_CTX_size(s->write_hash);
		if (mac_size < 0)
			goto err;
		}

	 
#if 0
	 
	if (!clear && !create_empty_fragment && !s->s3->empty_fragment_done
	    && SSL_version(s) != DTLS1_VERSION && SSL_version(s) != DTLS1_BAD_VER)
		{
		 

		if (s->s3->need_empty_fragments && type == SSL3_RT_APPLICATION_DATA)
			{
			 
			prefix_len = s->method->do_ssl_write(s, type, buf, 0, 1);
			if (prefix_len <= 0)
				goto err;

			if (s->s3->wbuf.len < (size_t)prefix_len + SSL3_RT_MAX_PACKET_SIZE)
				{
				 
				SSLerr(SSL_F_DO_DTLS1_WRITE, ERR_R_INTERNAL_ERROR);
				goto err;
				}
			}
		
		s->s3->empty_fragment_done = 1;
		}
#endif
	p = wb->buf + prefix_len;

	 

	*(p++)=type&0xff;
	wr->type=type;
	 
	if (s->method->version == DTLS_ANY_VERSION)
		{
		*(p++)=DTLS1_VERSION>>8;
		*(p++)=DTLS1_VERSION&0xff;
		}
	else
		{
		*(p++)=s->version>>8;
		*(p++)=s->version&0xff;
		}

	 
	pseq=p; 
	p+=10;

	 
	if (s->enc_write_ctx)
		{
		int mode = EVP_CIPHER_CTX_mode(s->enc_write_ctx);
		if (mode == EVP_CIPH_CBC_MODE)
			{
			eivlen = EVP_CIPHER_CTX_iv_length(s->enc_write_ctx);
			if (eivlen <= 1)
				eivlen = 0;
			}
		 
		else if (mode == EVP_CIPH_GCM_MODE)
			eivlen = EVP_GCM_TLS_EXPLICIT_IV_LEN;
		else
			eivlen = 0;
		}
	else 
		eivlen = 0;

	 
	wr->data=p + eivlen;   
	wr->length=(int)len;
	wr->input=(unsigned char *)buf;

	 

	 
	if (s->compress != NULL)
		{
		if (!ssl3_do_compress(s))
			{
			SSLerr(SSL_F_DO_DTLS1_WRITE,SSL_R_COMPRESSION_FAILURE);
			goto err;
			}
		}
	else
		{
		memcpy(wr->data,wr->input,wr->length);
		wr->input=wr->data;
		}

	 

	if (mac_size != 0)
		{
		if(s->method->ssl3_enc->mac(s,&(p[wr->length + eivlen]),1) < 0)
			goto err;
		wr->length+=mac_size;
		}

	 
	wr->input=p;
	wr->data=p;

	if (eivlen)
		wr->length += eivlen;

	if(s->method->ssl3_enc->enc(s,1) < 1) goto err;

	 
 
	
	 
	
	s2n(s->d1->w_epoch, pseq);

	 
 

	memcpy(pseq, &(s->s3->write_sequence[2]), 6);
	pseq+=6;
	s2n(wr->length,pseq);

	if (s->msg_callback)
		s->msg_callback(1, 0, SSL3_RT_HEADER, pseq - DTLS1_RT_HEADER_LENGTH, DTLS1_RT_HEADER_LENGTH, s, s->msg_callback_arg);

	 
	wr->type=type;  
	wr->length+=DTLS1_RT_HEADER_LENGTH;

#if 0   
	 
	if ( type == SSL3_RT_HANDSHAKE || type == SSL3_RT_CHANGE_CIPHER_SPEC)
		dtls1_buffer_record(s, wr->data, wr->length, 
			*((PQ_64BIT *)&(s->s3->write_sequence[0])));
#endif

	ssl3_record_sequence_update(&(s->s3->write_sequence[0]));

	if (create_empty_fragment)
		{
		 
		return wr->length;
		}

	 
	wb->left = prefix_len + wr->length;
	wb->offset = 0;

	 
	s->s3->wpend_tot=len;
	s->s3->wpend_buf=buf;
	s->s3->wpend_type=type;
	s->s3->wpend_ret=len;

	 
	return ssl3_write_pending(s,type,buf,len);
err:
	return -1;
	}
