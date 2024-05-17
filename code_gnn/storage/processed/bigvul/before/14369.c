int dtls1_shutdown(SSL *s)
	{
	int ret;
#ifndef OPENSSL_NO_SCTP
	if (BIO_dgram_is_sctp(SSL_get_wbio(s)) &&
	    !(s->shutdown & SSL_SENT_SHUTDOWN))
		{
		ret = BIO_dgram_sctp_wait_for_dry(SSL_get_wbio(s));
		if (ret < 0) return -1;

		if (ret == 0)
			BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_SAVE_SHUTDOWN, 1, NULL);
		}
#endif
	ret = ssl3_shutdown(s);
#ifndef OPENSSL_NO_SCTP
	BIO_ctrl(SSL_get_wbio(s), BIO_CTRL_DGRAM_SCTP_SAVE_SHUTDOWN, 0, NULL);
#endif
	return ret;
	}
