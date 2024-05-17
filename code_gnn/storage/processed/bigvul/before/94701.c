static int ssl3_get_record(SSL *s)
	{
	int ssl_major,ssl_minor,al;
	int enc_err,n,i,ret= -1;
	SSL3_RECORD *rr;
	SSL_SESSION *sess;
	unsigned char *p;
	unsigned char md[EVP_MAX_MD_SIZE];
	short version;
	unsigned mac_size;
	size_t extra;
	unsigned empty_record_count = 0;

	rr= &(s->s3->rrec);
	sess=s->session;

	if (s->options & SSL_OP_MICROSOFT_BIG_SSLV3_BUFFER)
		extra=SSL3_RT_MAX_EXTRA;
	else
		extra=0;
	if (extra && !s->s3->init_extra)
		{
		 
		SSLerr(SSL_F_SSL3_GET_RECORD, ERR_R_INTERNAL_ERROR);
		return -1;
		}

again:
	 
	if (	(s->rstate != SSL_ST_READ_BODY) ||
		(s->packet_length < SSL3_RT_HEADER_LENGTH)) 
		{
		n=ssl3_read_n(s, SSL3_RT_HEADER_LENGTH, s->s3->rbuf.len, 0);
		if (n <= 0) return(n);  
		s->rstate=SSL_ST_READ_BODY;

		p=s->packet;
		if (s->msg_callback)
			s->msg_callback(0, 0, SSL3_RT_HEADER, p, 5, s, s->msg_callback_arg);

		 
		rr->type= *(p++);
		ssl_major= *(p++);
		ssl_minor= *(p++);
		version=(ssl_major<<8)|ssl_minor;
		n2s(p,rr->length);
#if 0
fprintf(stderr, "Record type=%d, Length=%d\n", rr->type, rr->length);
#endif

		 
		if (!s->first_packet)
			{
			if (version != s->version)
				{
				SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_WRONG_VERSION_NUMBER);
                                if ((s->version & 0xFF00) == (version & 0xFF00) && !s->enc_write_ctx && !s->write_hash)
                                	 
					s->version = (unsigned short)version;
				al=SSL_AD_PROTOCOL_VERSION;
				goto f_err;
				}
			}

		if ((version>>8) != SSL3_VERSION_MAJOR)
			{
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_WRONG_VERSION_NUMBER);
			goto err;
			}

		if (rr->length > s->s3->rbuf.len - SSL3_RT_HEADER_LENGTH)
			{
			al=SSL_AD_RECORD_OVERFLOW;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_PACKET_LENGTH_TOO_LONG);
			goto f_err;
			}

		 
		}

	 

	if (rr->length > s->packet_length-SSL3_RT_HEADER_LENGTH)
		{
		 
		i=rr->length;
		n=ssl3_read_n(s,i,i,1);
		if (n <= 0) return(n);  
		 
		}

	s->rstate=SSL_ST_READ_HEADER;  

	 
	rr->input= &(s->packet[SSL3_RT_HEADER_LENGTH]);

	  

	 

	 
	if (rr->length > SSL3_RT_MAX_ENCRYPTED_LENGTH+extra)
		{
		al=SSL_AD_RECORD_OVERFLOW;
		SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_ENCRYPTED_LENGTH_TOO_LONG);
		goto f_err;
		}

	 
	rr->data=rr->input;
	rr->orig_len=rr->length;
	 
	if (SSL_USE_ETM(s) && s->read_hash)
		{
		unsigned char *mac;
		mac_size=EVP_MD_CTX_size(s->read_hash);
		OPENSSL_assert(mac_size <= EVP_MAX_MD_SIZE);
		if (rr->length < mac_size)
			{
			al=SSL_AD_DECODE_ERROR;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_LENGTH_TOO_SHORT);
			goto f_err;
			}
		rr->length -= mac_size;
		mac = rr->data + rr->length;
		i=s->method->ssl3_enc->mac(s,md,0  );
		if (i < 0 || CRYPTO_memcmp(md, mac, (size_t)mac_size) != 0)
			{
			al=SSL_AD_BAD_RECORD_MAC;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_DECRYPTION_FAILED_OR_BAD_RECORD_MAC);
			goto f_err;
			}
		}

	enc_err = s->method->ssl3_enc->enc(s,0);
	 
	if (enc_err == 0)
		{
		al=SSL_AD_DECRYPTION_FAILED;
		SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_BLOCK_CIPHER_PAD_IS_WRONG);
		goto f_err;
		}

#ifdef TLS_DEBUG
printf("dec %d\n",rr->length);
{ unsigned int z; for (z=0; z<rr->length; z++) printf("%02X%c",rr->data[z],((z+1)%16)?' ':'\n'); }
printf("\n");
#endif

	 
	if ((sess != NULL) &&
	    (s->enc_read_ctx != NULL) &&
	    (EVP_MD_CTX_md(s->read_hash) != NULL) && !SSL_USE_ETM(s))
		{
		 
		unsigned char *mac = NULL;
		unsigned char mac_tmp[EVP_MAX_MD_SIZE];
		mac_size=EVP_MD_CTX_size(s->read_hash);
		OPENSSL_assert(mac_size <= EVP_MAX_MD_SIZE);

		 
		if (rr->orig_len < mac_size ||
		     
		    (EVP_CIPHER_CTX_mode(s->enc_read_ctx) == EVP_CIPH_CBC_MODE &&
		     rr->orig_len < mac_size+1))
			{
			al=SSL_AD_DECODE_ERROR;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_LENGTH_TOO_SHORT);
			goto f_err;
			}

		if (EVP_CIPHER_CTX_mode(s->enc_read_ctx) == EVP_CIPH_CBC_MODE)
			{
			 
			mac = mac_tmp;
			ssl3_cbc_copy_mac(mac_tmp, rr, mac_size);
			rr->length -= mac_size;
			}
		else
			{
			 
			rr->length -= mac_size;
			mac = &rr->data[rr->length];
			}

		i=s->method->ssl3_enc->mac(s,md,0  );
		if (i < 0 || mac == NULL || CRYPTO_memcmp(md, mac, (size_t)mac_size) != 0)
			enc_err = -1;
		if (rr->length > SSL3_RT_MAX_COMPRESSED_LENGTH+extra+mac_size)
			enc_err = -1;
		}

	if (enc_err < 0)
		{
		 
		al=SSL_AD_BAD_RECORD_MAC;
		SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_DECRYPTION_FAILED_OR_BAD_RECORD_MAC);
		goto f_err;
		}

	 
	if (s->expand != NULL)
		{
		if (rr->length > SSL3_RT_MAX_COMPRESSED_LENGTH+extra)
			{
			al=SSL_AD_RECORD_OVERFLOW;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_COMPRESSED_LENGTH_TOO_LONG);
			goto f_err;
			}
		if (!ssl3_do_uncompress(s))
			{
			al=SSL_AD_DECOMPRESSION_FAILURE;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_BAD_DECOMPRESSION);
			goto f_err;
			}
		}

	if (rr->length > SSL3_RT_MAX_PLAIN_LENGTH+extra)
		{
		al=SSL_AD_RECORD_OVERFLOW;
		SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_DATA_LENGTH_TOO_LONG);
		goto f_err;
		}

	rr->off=0;
	 

	 
	s->packet_length=0;

	 
	if (rr->length == 0)
		{
		empty_record_count++;
		if (empty_record_count > MAX_EMPTY_RECORDS)
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_SSL3_GET_RECORD,SSL_R_RECORD_TOO_SMALL);
			goto f_err;
			}
		goto again;
		}

#if 0
fprintf(stderr, "Ultimate Record type=%d, Length=%d\n", rr->type, rr->length);
#endif

	return(1);

f_err:
	ssl3_send_alert(s,SSL3_AL_FATAL,al);
err:
	return(ret);
	}
