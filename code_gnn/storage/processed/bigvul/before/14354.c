dtls1_heartbeat(SSL *s)
	{
	unsigned char *buf, *p;
	int ret;
	unsigned int payload = 18;  
	unsigned int padding = 16;  

	 
	if (!(s->tlsext_heartbeat & SSL_TLSEXT_HB_ENABLED) ||
	    s->tlsext_heartbeat & SSL_TLSEXT_HB_DONT_SEND_REQUESTS)
		{
		SSLerr(SSL_F_DTLS1_HEARTBEAT,SSL_R_TLS_HEARTBEAT_PEER_DOESNT_ACCEPT);
		return -1;
		}

	 
	if (s->tlsext_hb_pending)
		{
		SSLerr(SSL_F_DTLS1_HEARTBEAT,SSL_R_TLS_HEARTBEAT_PENDING);
		return -1;
		}

	 
	if (SSL_in_init(s) || s->in_handshake)
		{
		SSLerr(SSL_F_DTLS1_HEARTBEAT,SSL_R_UNEXPECTED_MESSAGE);
		return -1;
		}

	 
	OPENSSL_assert(payload + padding <= 16381);

	 
	buf = OPENSSL_malloc(1 + 2 + payload + padding);
	p = buf;
	 
	*p++ = TLS1_HB_REQUEST;
	 
	s2n(payload, p);
	 
	s2n(s->tlsext_hb_seq, p);
	 
	RAND_pseudo_bytes(p, 16);
	p += 16;
	 
	RAND_pseudo_bytes(p, padding);

	ret = dtls1_write_bytes(s, TLS1_RT_HEARTBEAT, buf, 3 + payload + padding);
	if (ret >= 0)
		{
		if (s->msg_callback)
			s->msg_callback(1, s->version, TLS1_RT_HEARTBEAT,
				buf, 3 + payload + padding,
				s, s->msg_callback_arg);

		dtls1_start_timer(s);
		s->tlsext_hb_pending = 1;
		}

	OPENSSL_free(buf);

	return ret;
	}
