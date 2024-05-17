int ssl3_send_alert(SSL *s, int level, int desc)
	{
	 
	desc=s->method->ssl3_enc->alert_value(desc);
	if (s->version == SSL3_VERSION && desc == SSL_AD_PROTOCOL_VERSION)
		desc = SSL_AD_HANDSHAKE_FAILURE;  
	if (desc < 0) return -1;
	 
	if ((level == SSL3_AL_FATAL) && (s->session != NULL))
		SSL_CTX_remove_session(s->ctx,s->session);

	s->s3->alert_dispatch=1;
	s->s3->send_alert[0]=level;
	s->s3->send_alert[1]=desc;
	if (s->s3->wbuf.left == 0)  
		return s->method->ssl_dispatch_alert(s);
	 
	return -1;
	}
