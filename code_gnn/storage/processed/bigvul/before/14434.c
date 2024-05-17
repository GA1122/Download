dtls1_get_message_fragment(SSL *s, int st1, int stn, long max, int *ok)
	{
	unsigned char wire[DTLS1_HM_HEADER_LENGTH];
	unsigned long len, frag_off, frag_len;
	int i,al;
	struct hm_header_st msg_hdr;

	 
	if ((frag_len = dtls1_retrieve_buffered_fragment(s,max,ok)) || *ok)
		{
		if (*ok)	s->init_num = frag_len;
		return frag_len;
		}

	 
	i=s->method->ssl_read_bytes(s,SSL3_RT_HANDSHAKE,wire,
		DTLS1_HM_HEADER_LENGTH, 0);
	if (i <= 0) 	 
		{
		s->rwstate=SSL_READING;
		*ok = 0;
		return i;
		}
	 
	if (i != DTLS1_HM_HEADER_LENGTH)
		{
		al=SSL_AD_UNEXPECTED_MESSAGE;
		SSLerr(SSL_F_DTLS1_GET_MESSAGE_FRAGMENT,SSL_R_UNEXPECTED_MESSAGE);
		goto f_err;
		}

	 
	dtls1_get_message_header(wire, &msg_hdr);

	 
	if (msg_hdr.seq != s->d1->handshake_read_seq && !(s->d1->listen && msg_hdr.seq == 1))
		return dtls1_process_out_of_seq_message(s, &msg_hdr, ok);

	len = msg_hdr.msg_len;
	frag_off = msg_hdr.frag_off;
	frag_len = msg_hdr.frag_len;

	if (frag_len && frag_len < len)
		return dtls1_reassemble_fragment(s, &msg_hdr, ok);

	if (!s->server && s->d1->r_msg_hdr.frag_off == 0 &&
		wire[0] == SSL3_MT_HELLO_REQUEST)
		{
		 
		if (wire[1] == 0 && wire[2] == 0 && wire[3] == 0)
			{
			if (s->msg_callback)
				s->msg_callback(0, s->version, SSL3_RT_HANDSHAKE, 
					wire, DTLS1_HM_HEADER_LENGTH, s, 
					s->msg_callback_arg);
			
			s->init_num = 0;
			return dtls1_get_message_fragment(s, st1, stn,
				max, ok);
			}
		else  
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_DTLS1_GET_MESSAGE_FRAGMENT,SSL_R_UNEXPECTED_MESSAGE);
			goto f_err;
			}
		}

	if ((al=dtls1_preprocess_fragment(s,&msg_hdr,max)))
		goto f_err;

	 
	s->state=stn;

	if ( frag_len > 0)
		{
		unsigned char *p=(unsigned char *)s->init_buf->data+DTLS1_HM_HEADER_LENGTH;

		i=s->method->ssl_read_bytes(s,SSL3_RT_HANDSHAKE,
			&p[frag_off],frag_len,0);
		 
		if (i <= 0)
			{
			s->rwstate=SSL_READING;
			*ok = 0;
			return i;
			}
		}
	else
		i = 0;

	 
	if (i != (int)frag_len)
		{
		al=SSL3_AD_ILLEGAL_PARAMETER;
		SSLerr(SSL_F_DTLS1_GET_MESSAGE_FRAGMENT,SSL3_AD_ILLEGAL_PARAMETER);
		goto f_err;
		}

	*ok = 1;

	 
	s->init_num = frag_len;
	return frag_len;

f_err:
	ssl3_send_alert(s,SSL3_AL_FATAL,al);
	s->init_num = 0;

	*ok=0;
	return(-1);
	}
