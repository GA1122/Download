 int ssl23_get_client_hello(SSL *s)
 	{
 	char buf_space[11];  
 	char *buf= &(buf_space[0]);
 	unsigned char *p,*d,*d_len,*dd;
 	unsigned int i;
 	unsigned int csl,sil,cl;
 	int n=0,j;
 	int type=0;
 	int v[2];
 
 	if (s->state ==	SSL23_ST_SR_CLNT_HELLO_A)
 		{
 		 
 		v[0]=v[1]=0;
 
 		if (!ssl3_setup_buffers(s)) goto err;
 
 		n=ssl23_read_bytes(s, sizeof buf_space);
 		if (n != sizeof buf_space) return(n);  
 
 		p=s->packet;
 
 		memcpy(buf,p,n);
 
 		if ((p[0] & 0x80) && (p[2] == SSL2_MT_CLIENT_HELLO))
 			{
 			 
 			if ((p[3] == 0x00) && (p[4] == 0x02))
 				{
 				v[0]=p[3]; v[1]=p[4];
 				 
 				if (!(s->options & SSL_OP_NO_SSLv2))
 					type=1;
 				}
 			else if (p[3] == SSL3_VERSION_MAJOR)
 				{
 				v[0]=p[3]; v[1]=p[4];
 				 
 				if (p[4] >= TLS1_VERSION_MINOR)
 					{
 					if (p[4] >= TLS1_2_VERSION_MINOR &&
 					   !(s->options & SSL_OP_NO_TLSv1_2))
 						{
 						s->version=TLS1_2_VERSION;
 						s->state=SSL23_ST_SR_CLNT_HELLO_B;
 						}
 					else if (p[4] >= TLS1_1_VERSION_MINOR &&
 					   !(s->options & SSL_OP_NO_TLSv1_1))
 						{
 						s->version=TLS1_1_VERSION;
 						   
 						s->state=SSL23_ST_SR_CLNT_HELLO_B;
 						}
 					else if (!(s->options & SSL_OP_NO_TLSv1))
 						{
 						s->version=TLS1_VERSION;
 						   
 						s->state=SSL23_ST_SR_CLNT_HELLO_B;
 						}
 					else if (!(s->options & SSL_OP_NO_SSLv3))
 						{
 						s->version=SSL3_VERSION;
 						 
 						s->state=SSL23_ST_SR_CLNT_HELLO_B;
 						}
 					else if (!(s->options & SSL_OP_NO_SSLv2))
 						{
 						type=1;
 						}
 					}
 				else if (!(s->options & SSL_OP_NO_SSLv3))
 					{
 					s->version=SSL3_VERSION;
 					 
 					s->state=SSL23_ST_SR_CLNT_HELLO_B;
 					}
 				else if (!(s->options & SSL_OP_NO_SSLv2))
 					type=1;
 
 				}
 			}
 		else if ((p[0] == SSL3_RT_HANDSHAKE) &&
 			 (p[1] == SSL3_VERSION_MAJOR) &&
 			 (p[5] == SSL3_MT_CLIENT_HELLO) &&
 			 ((p[3] == 0 && p[4] < 5  )
 				|| (p[9] >= p[1])))
 			{
 			 
 			
 			v[0]=p[1];  
 			 
                         if (p[3] == 0 && p[4] < 6)
                                 {
#if 0
                                 SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_RECORD_TOO_SMALL);
                                 goto err;
#else
                               v[1] = TLS1_VERSION_MINOR;
#endif
                                 }
                          
                       else if (p[9] > SSL3_VERSION_MAJOR)
//                        if (p[9] > SSL3_VERSION_MAJOR)
                                 v[1]=0xff;
                         else
                                 v[1]=p[10];  
 			else if (p[9] > SSL3_VERSION_MAJOR)
 				v[1]=0xff;
 			else
 				v[1]=p[10];  
 			if (v[1] >= TLS1_VERSION_MINOR)
 				{
 				if (v[1] >= TLS1_2_VERSION_MINOR &&
 					!(s->options & SSL_OP_NO_TLSv1_2))
 					{
 					s->version=TLS1_2_VERSION;
 					type=3;
 					}
 				else if (v[1] >= TLS1_1_VERSION_MINOR &&
 					!(s->options & SSL_OP_NO_TLSv1_1))
 					{
 					s->version=TLS1_1_VERSION;
 					type=3;
 					}
 				else if (!(s->options & SSL_OP_NO_TLSv1))
 					{
 					s->version=TLS1_VERSION;
 					type=3;
 					}
 				else if (!(s->options & SSL_OP_NO_SSLv3))
 					{
 					s->version=SSL3_VERSION;
 					type=3;
 					}
 				}
 			else
 				{
 				 
 				if (!(s->options & SSL_OP_NO_SSLv3))
 					{
 					s->version=SSL3_VERSION;
 					type=3;
 					}
 				else if (!(s->options & SSL_OP_NO_TLSv1))
 					{
 					 
 					s->version=TLS1_VERSION;
 					type=3;
 					}
 				}
 			}
 		else if ((strncmp("GET ", (char *)p,4) == 0) ||
 			 (strncmp("POST ",(char *)p,5) == 0) ||
 			 (strncmp("HEAD ",(char *)p,5) == 0) ||
 			 (strncmp("PUT ", (char *)p,4) == 0))
 			{
 			SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_HTTP_REQUEST);
 			goto err;
 			}
 		else if (strncmp("CONNECT",(char *)p,7) == 0)
 			{
 			SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_HTTPS_PROXY_REQUEST);
 			goto err;
 			}
 		}
 
 	if (s->version < TLS1_2_VERSION && tls1_suiteb(s))
 		{
 		SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,
 				SSL_R_ONLY_TLS_1_2_ALLOWED_IN_SUITEB_MODE);
 		goto err;
 		}
 
 #ifdef OPENSSL_FIPS
 	if (FIPS_mode() && (s->version < TLS1_VERSION))
 		{
 		SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,
 					SSL_R_ONLY_TLS_ALLOWED_IN_FIPS_MODE);
 		goto err;
 		}
 #endif
 
 	if (!ssl_security(s, SSL_SECOP_VERSION, 0, s->version, NULL))
 		{
 		SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_VERSION_TOO_LOW);
 		goto err;
 		}
 
 	if (s->state == SSL23_ST_SR_CLNT_HELLO_B)
 		{
 		 
                 v[1] = p[4];
  
//                 
                 n=((p[0]&0x7f)<<8)|p[1];
                 if (n > (1024*4))
                         {
                         SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_RECORD_TOO_LARGE);
                         goto err;
                         }
//                if (n < 9)
//                        {
//                        SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_RECORD_LENGTH_MISMATCH);
//                        goto err;
//                        }
  
                 j=ssl23_read_bytes(s,n+2);
//                 
                 if (j <= 0) return(j);
  
                 ssl3_finish_mac(s, s->packet+2, s->packet_length-2);
 
 		 
 		*(d++) = SSL3_MT_CLIENT_HELLO;
 		 
 		d_len = d;
 		d += 3;
 
 		 
 		*(d++) = SSL3_VERSION_MAJOR;  
 		*(d++) = v[1];
 
 		 
 		 
 		i=(cl > SSL3_RANDOM_SIZE)?SSL3_RANDOM_SIZE:cl;
 		memset(d,0,SSL3_RANDOM_SIZE);
 		memcpy(&(d[SSL3_RANDOM_SIZE-i]),&(p[csl+sil]),i);
 		d+=SSL3_RANDOM_SIZE;
 
 		 
 		*(d++)=0;
 
 		 
 		j=0;
 		dd=d;
 		d+=2;
 		for (i=0; i<csl; i+=3)
 			{
 			if (p[i] != 0) continue;
 			*(d++)=p[i+1];
 			*(d++)=p[i+2];
 			j+=2;
 			}
 		s2n(j,dd);
 
 		 
 		*(d++)=1;
 		*(d++)=0;
 		
 #if 0
                  
 	        p = p+csl+sil+cl;
 		while (p <  s->packet+s->packet_length)
 			{
 			*(d++)=*(p++);
 			}
 #endif
 
 		i = (d-(unsigned char *)s->init_buf->data) - 4;
 		l2n3((long)i, d_len);
 
 		 
 		s->s3->tmp.reuse_message=1;
 		s->s3->tmp.message_type=SSL3_MT_CLIENT_HELLO;
 		s->s3->tmp.message_size=i;
 		}
 
 	 
 	 
 
 	if (type == 1)
 		{
 #ifdef OPENSSL_NO_SSL2
 		SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_UNSUPPORTED_PROTOCOL);
 		goto err;
 #else
 		 
 		 
 
 		if (s->s2 == NULL)
 			{
 			if (!ssl2_new(s))
 				goto err;
 			}
 		else
 			ssl2_clear(s);
 
 		if (s->s3 != NULL) ssl3_free(s);
 
 		if (!BUF_MEM_grow_clean(s->init_buf,
 			SSL2_MAX_RECORD_LENGTH_3_BYTE_HEADER))
 			{
 			goto err;
 			}
 
 		s->state=SSL2_ST_GET_CLIENT_HELLO_A;
 		if (s->options & SSL_OP_NO_TLSv1 && s->options & SSL_OP_NO_SSLv3)
 			s->s2->ssl2_rollback=0;
 		else
 			 
 			s->s2->ssl2_rollback=1;
 
 		 
 		s->rstate=SSL_ST_READ_HEADER;
 		s->packet_length=n;
 		s->packet= &(s->s2->rbuf[0]);
 		memcpy(s->packet,buf,n);
 		s->s2->rbuf_left=n;
 		s->s2->rbuf_offs=0;
 
 		s->method=SSLv2_server_method();
 		s->handshake_func=s->method->ssl_accept;
 #endif
 		}
 
 	if ((type == 2) || (type == 3))
 		{
 		 
 
 		if (!ssl_init_wbio_buffer(s,1)) goto err;
 
 		 
 		s->state=SSL3_ST_SR_CLNT_HELLO_A;
 
 		if (type == 3)
 			{
 			 
 			s->rstate=SSL_ST_READ_HEADER;
 			s->packet_length=n;
 			if (s->s3->rbuf.buf == NULL)
 				if (!ssl3_setup_read_buffer(s))
 					goto err;
 
 			s->packet= &(s->s3->rbuf.buf[0]);
 			memcpy(s->packet,buf,n);
 			s->s3->rbuf.left=n;
 			s->s3->rbuf.offset=0;
 			}
 		else
 			{
 			s->packet_length=0;
 			s->s3->rbuf.left=0;
 			s->s3->rbuf.offset=0;
 			}
 		if (s->version == TLS1_2_VERSION)
 			s->method = TLSv1_2_server_method();
 		else if (s->version == TLS1_1_VERSION)
 			s->method = TLSv1_1_server_method();
 		else if (s->version == TLS1_VERSION)
 			s->method = TLSv1_server_method();
 		else
 			s->method = SSLv3_server_method();
 #if 0  
 		s->client_version=(v[0]<<8)|v[1];
 #endif
 		s->handshake_func=s->method->ssl_accept;
 		}
 	
 	if ((type < 1) || (type > 3))
 		{
 		 
 		SSLerr(SSL_F_SSL23_GET_CLIENT_HELLO,SSL_R_UNKNOWN_PROTOCOL);
 		goto err;
 		}
 	s->init_num=0;
 
 	if (buf != buf_space) OPENSSL_free(buf);
 	return(SSL_accept(s));
 err:
 	if (buf != buf_space) OPENSSL_free(buf);
 	return(-1);
 	}