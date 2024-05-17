int dtls1_read_failed(SSL *s, int code)
	{
	if ( code > 0)
		{
		fprintf( stderr, "invalid state reached %s:%d", __FILE__, __LINE__);
		return 1;
		}

	if (!dtls1_is_timer_expired(s))
		{
		 
		return code;
		}

#ifndef OPENSSL_NO_HEARTBEATS
	if (!SSL_in_init(s) && !s->tlsext_hb_pending)   
#else
	if (!SSL_in_init(s))   
#endif
		{
		BIO_set_flags(SSL_get_rbio(s), BIO_FLAGS_READ);
		return code;
		}

#if 0  
	item = pqueue_peek(state->rcvd_records);
	if ( item )
		{
		 
		}
	else
#endif

#if 0   
	if ( state->timeout.read_timeouts >= DTLS1_TMO_READ_COUNT)
		ssl3_send_alert(s,SSL3_AL_WARNING,
			DTLS1_AD_MISSING_HANDSHAKE_MESSAGE);
#endif

	return dtls1_handle_timeout(s);
	}
