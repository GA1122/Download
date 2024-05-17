int ssl3_send_cert_status(SSL *s)
	{
	if (s->state == SSL3_ST_SW_CERT_STATUS_A)
		{
		unsigned char *p;
		 
		if (!BUF_MEM_grow(s->init_buf, 8 + s->tlsext_ocsp_resplen))
			return -1;

		p=(unsigned char *)s->init_buf->data;

		 
		*(p++)=SSL3_MT_CERTIFICATE_STATUS;
		 
		l2n3(s->tlsext_ocsp_resplen + 4, p);
		 
		*(p++)= s->tlsext_status_type;
		 
		l2n3(s->tlsext_ocsp_resplen, p);
		 
		memcpy(p, s->tlsext_ocsp_resp, s->tlsext_ocsp_resplen);
		 
		s->init_num = 8 + s->tlsext_ocsp_resplen;
		s->state=SSL3_ST_SW_CERT_STATUS_B;
		s->init_off = 0;
		}

	 
	return(ssl3_do_write(s,SSL3_RT_HANDSHAKE));
	}
