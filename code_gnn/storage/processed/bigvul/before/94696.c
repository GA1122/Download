static int do_ssl3_write(SSL *s, int type, const unsigned char *buf,
			 unsigned int len, int create_empty_fragment)
	{
	unsigned char *p,*plen;
	int i,mac_size,clear=0;
	int prefix_len=0;
	int eivlen;
	long align=0;
	SSL3_RECORD *wr;
	SSL3_BUFFER *wb=&(s->s3->wbuf);
	SSL_SESSION *sess;


	 
	if (wb->left != 0)
		return(ssl3_write_pending(s,type,buf,len));

	 
	if (s->s3->alert_dispatch)
		{
		i=s->method->ssl_dispatch_alert(s);
		if (i <= 0)
			return(i);
		 
		}

 	if (wb->buf == NULL)
		if (!ssl3_setup_write_buffer(s))
			return -1;

	if (len == 0 && !create_empty_fragment)
		return 0;

	wr= &(s->s3->wrec);
	sess=s->session;

	if (	(sess == NULL) ||
		(s->enc_write_ctx == NULL) ||
		(EVP_MD_CTX_md(s->write_hash) == NULL))
		{
#if 1
		clear=s->enc_write_ctx?0:1;	 
#else
		clear=1;
#endif
		mac_size=0;
		}
	else
		{
		mac_size=EVP_MD_CTX_size(s->write_hash);
		if (mac_size < 0)
			goto err;
		}

#if 0 && !defined(OPENSSL_NO_MULTIBLOCK) && EVP_CIPH_FLAG_TLS1_1_MULTIBLOCK
	if (type==SSL3_RT_APPLICATION_DATA && s->compress==NULL &&
	    !SSL_USE_ETM(s) && SSL_USE_EXPLICIT_IV(s) &&  
	    EVP_CIPHER_flags(s->enc_write_ctx->cipher)&EVP_CIPH_FLAG_TLS1_1_MULTIBLOCK)
		do {
		unsigned char aad[13];
		EVP_CTRL_TLS1_1_MULTIBLOCK_PARAM mb_param = {NULL,aad,sizeof(aad),0};
		int packlen;

		memcpy(aad,s->s3->write_sequence,8);
		aad[8]=type;
		aad[9]=(unsigned char)(s->version>>8);
		aad[10]=(unsigned char)(s->version);
		aad[11]=(unsigned char)(len>>8);
		aad[12]=(unsigned char)len;
		packlen = EVP_CIPHER_CTX_ctrl(s->enc_write_ctx,
				EVP_CTRL_TLS1_1_MULTIBLOCK_AAD,
				sizeof(mb_param),&mb_param);

		if (packlen==0 || packlen > wb->len) break;

		mb_param.out = wb->buf;
		mb_param.inp = buf;
		mb_param.len = len;
		EVP_CIPHER_CTX_ctrl(s->enc_write_ctx,
				EVP_CTRL_TLS1_1_MULTIBLOCK_ENCRYPT,
				sizeof(mb_param),&mb_param);

		s->s3->write_sequence[7] += mb_param.interleave;
		if (s->s3->write_sequence[7] < mb_param.interleave)
			{
			int j=6;
			while (j>=0 && (++s->s3->write_sequence[j--])==0) ;
			}

		wb->offset=0;
		wb->left = packlen;

		 
		s->s3->wpend_tot=len;
		s->s3->wpend_buf=buf;
		s->s3->wpend_type=type;
		s->s3->wpend_ret=len;

		 
		return ssl3_write_pending(s,type,buf,len);
		} while (0);
#endif

	 
	if (!clear && !create_empty_fragment && !s->s3->empty_fragment_done)
		{
		 

		if (s->s3->need_empty_fragments && type == SSL3_RT_APPLICATION_DATA)
			{
			 
			prefix_len = do_ssl3_write(s, type, buf, 0, 1);
			if (prefix_len <= 0)
				goto err;

			if (prefix_len >
		(SSL3_RT_HEADER_LENGTH + SSL3_RT_SEND_MAX_ENCRYPTED_OVERHEAD))
				{
				 
				SSLerr(SSL_F_DO_SSL3_WRITE, ERR_R_INTERNAL_ERROR);
				goto err;
				}
			}
		
		s->s3->empty_fragment_done = 1;
		}

	if (create_empty_fragment)
		{
#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
		 
		align = (long)wb->buf + 2*SSL3_RT_HEADER_LENGTH;
		align = (-align)&(SSL3_ALIGN_PAYLOAD-1);
#endif
		p = wb->buf + align;
		wb->offset  = align;
		}
	else if (prefix_len)
		{
		p = wb->buf + wb->offset + prefix_len;
		}
	else
		{
#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
		align = (long)wb->buf + SSL3_RT_HEADER_LENGTH;
		align = (-align)&(SSL3_ALIGN_PAYLOAD-1);
#endif
		p = wb->buf + align;
		wb->offset  = align;
		}

	 

	*(p++)=type&0xff;
	wr->type=type;

	*(p++)=(s->version>>8);
	 
	if (s->state == SSL3_ST_CW_CLNT_HELLO_B
				&& !s->renegotiate
				&& TLS1_get_version(s) > TLS1_VERSION)
		*(p++) = 0x1;
	else
		*(p++)=s->version&0xff;

	 
	plen=p; 
	p+=2;
	 
	if (s->enc_write_ctx && SSL_USE_EXPLICIT_IV(s))
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
			SSLerr(SSL_F_DO_SSL3_WRITE,SSL_R_COMPRESSION_FAILURE);
			goto err;
			}
		}
	else
		{
		memcpy(wr->data,wr->input,wr->length);
		wr->input=wr->data;
		}

	 

	if (!SSL_USE_ETM(s) && mac_size != 0)
		{
		if (s->method->ssl3_enc->mac(s,&(p[wr->length + eivlen]),1) < 0)
			goto err;
		wr->length+=mac_size;
		}

	wr->input=p;
	wr->data=p;

	if (eivlen)
		{
	 
		wr->length += eivlen;
		}

	if(s->method->ssl3_enc->enc(s,1)<1) goto err;

	if (SSL_USE_ETM(s) && mac_size != 0)
		{
		if (s->method->ssl3_enc->mac(s,p + wr->length,1) < 0)
			goto err;
		wr->length+=mac_size;
		}

	 
	s2n(wr->length,plen);

	if (s->msg_callback)
		s->msg_callback(1, 0, SSL3_RT_HEADER, plen - 5, 5, s, s->msg_callback_arg);

	 
	wr->type=type;  
	wr->length+=SSL3_RT_HEADER_LENGTH;

	if (create_empty_fragment)
		{
		 
		return wr->length;
		}

	 
	wb->left = prefix_len + wr->length;

	 
	s->s3->wpend_tot=len;
	s->s3->wpend_buf=buf;
	s->s3->wpend_type=type;
	s->s3->wpend_ret=len;

	 
	return ssl3_write_pending(s,type,buf,len);
err:
	return -1;
	}
