 int ssl_parse_clienthello_tlsext(SSL *s, unsigned char **p, unsigned char *d, int n, int *al)
 	{
 	unsigned short type;
 	unsigned short size;
 	unsigned short len;
 	unsigned char *data = *p;
 	int renegotiate_seen = 0;
 	int sigalg_seen = 0;
 
 	s->servername_done = 0;
 	s->tlsext_status_type = -1;
 #ifndef OPENSSL_NO_NEXTPROTONEG
 	s->s3->next_proto_neg_seen = 0;
 #endif
 
 #ifndef OPENSSL_NO_HEARTBEATS
 	s->tlsext_heartbeat &= ~(SSL_TLSEXT_HB_ENABLED |
 	                       SSL_TLSEXT_HB_DONT_SEND_REQUESTS);
 #endif
 
 #ifndef OPENSSL_NO_EC
 	if (s->options & SSL_OP_SAFARI_ECDHE_ECDSA_BUG)
 		ssl_check_for_safari(s, data, d, n);
 #endif  
 
 	if (data >= (d+n-2))
 		goto ri_check;
 	n2s(data,len);
 
 	if (data > (d+n-len)) 
 		goto ri_check;
 
 	while (data <= (d+n-4))
 		{
 		n2s(data,type);
 		n2s(data,size);
 
 		if (data+size > (d+n))
 	   		goto ri_check;
 #if 0
 		fprintf(stderr,"Received extension type %d size %d\n",type,size);
 #endif
 		if (s->tlsext_debug_cb)
 			s->tlsext_debug_cb(s, 0, type, data, size,
 						s->tlsext_debug_arg);
        
 
 		if (type == TLSEXT_TYPE_server_name)
 			{
 			unsigned char *sdata;
 			int servname_type;
 			int dsize; 
 		
 			if (size < 2) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			n2s(data,dsize);  
 			size -= 2;
 			if (dsize > size  ) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				} 
 
 			sdata = data;
 			while (dsize > 3) 
 				{
 	 			servname_type = *(sdata++); 
 				n2s(sdata,len);
 				dsize -= 3;
 
 				if (len > dsize) 
 					{
 					*al = SSL_AD_DECODE_ERROR;
 					return 0;
 					}
 				if (s->servername_done == 0)
 				switch (servname_type)
 					{
 				case TLSEXT_NAMETYPE_host_name:
 					if (!s->hit)
 						{
 						if(s->session->tlsext_hostname)
 							{
 							*al = SSL_AD_DECODE_ERROR;
 							return 0;
 							}
 						if (len > TLSEXT_MAXLEN_host_name)
 							{
 							*al = TLS1_AD_UNRECOGNIZED_NAME;
 							return 0;
 							}
 						if ((s->session->tlsext_hostname = OPENSSL_malloc(len+1)) == NULL)
 							{
 							*al = TLS1_AD_INTERNAL_ERROR;
 							return 0;
 							}
 						memcpy(s->session->tlsext_hostname, sdata, len);
 						s->session->tlsext_hostname[len]='\0';
 						if (strlen(s->session->tlsext_hostname) != len) {
 							OPENSSL_free(s->session->tlsext_hostname);
 							s->session->tlsext_hostname = NULL;
 							*al = TLS1_AD_UNRECOGNIZED_NAME;
 							return 0;
 						}
 						s->servername_done = 1; 
 
 						}
 					else 
 						s->servername_done = s->session->tlsext_hostname
 							&& strlen(s->session->tlsext_hostname) == len 
 							&& strncmp(s->session->tlsext_hostname, (char *)sdata, len) == 0;
 					
 					break;
 
 				default:
 					break;
 					}
 				 
 				dsize -= len;
 				}
 			if (dsize != 0) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 
 			}
 #ifndef OPENSSL_NO_SRP
 		else if (type == TLSEXT_TYPE_srp)
 			{
 			if (size <= 0 || ((len = data[0])) != (size -1))
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			if (s->srp_ctx.login != NULL)
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			if ((s->srp_ctx.login = OPENSSL_malloc(len+1)) == NULL)
 				return -1;
 			memcpy(s->srp_ctx.login, &data[1], len);
 			s->srp_ctx.login[len]='\0';
   
 			if (strlen(s->srp_ctx.login) != len) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			}
 #endif
 
 #ifndef OPENSSL_NO_EC
 		else if (type == TLSEXT_TYPE_ec_point_formats)
 			{
 			unsigned char *sdata = data;
 			int ecpointformatlist_length = *(sdata++);
 
 			if (ecpointformatlist_length != size - 1)
 				{
 				*al = TLS1_AD_DECODE_ERROR;
 				return 0;
 				}
 			if (!s->hit)
 				{
 				if(s->session->tlsext_ecpointformatlist)
 					{
 					OPENSSL_free(s->session->tlsext_ecpointformatlist);
 					s->session->tlsext_ecpointformatlist = NULL;
 					}
 				s->session->tlsext_ecpointformatlist_length = 0;
 				if ((s->session->tlsext_ecpointformatlist = OPENSSL_malloc(ecpointformatlist_length)) == NULL)
 					{
 					*al = TLS1_AD_INTERNAL_ERROR;
 					return 0;
 					}
 				s->session->tlsext_ecpointformatlist_length = ecpointformatlist_length;
 				memcpy(s->session->tlsext_ecpointformatlist, sdata, ecpointformatlist_length);
 				}
 #if 0
 			fprintf(stderr,"ssl_parse_clienthello_tlsext s->session->tlsext_ecpointformatlist (length=%i) ", s->session->tlsext_ecpointformatlist_length);
 			sdata = s->session->tlsext_ecpointformatlist;
 			for (i = 0; i < s->session->tlsext_ecpointformatlist_length; i++)
 				fprintf(stderr,"%i ",*(sdata++));
 			fprintf(stderr,"\n");
 #endif
 			}
 		else if (type == TLSEXT_TYPE_elliptic_curves)
 			{
 			unsigned char *sdata = data;
 			int ellipticcurvelist_length = (*(sdata++) << 8);
 			ellipticcurvelist_length += (*(sdata++));
 
 			if (ellipticcurvelist_length != size - 2 ||
 				ellipticcurvelist_length < 1)
 				{
 				*al = TLS1_AD_DECODE_ERROR;
 				return 0;
 				}
 			if (!s->hit)
 				{
 				if(s->session->tlsext_ellipticcurvelist)
 					{
 					*al = TLS1_AD_DECODE_ERROR;
 					return 0;
 					}
 				s->session->tlsext_ellipticcurvelist_length = 0;
 				if ((s->session->tlsext_ellipticcurvelist = OPENSSL_malloc(ellipticcurvelist_length)) == NULL)
 					{
 					*al = TLS1_AD_INTERNAL_ERROR;
 					return 0;
 					}
 				s->session->tlsext_ellipticcurvelist_length = ellipticcurvelist_length;
 				memcpy(s->session->tlsext_ellipticcurvelist, sdata, ellipticcurvelist_length);
 				}
 #if 0
 			fprintf(stderr,"ssl_parse_clienthello_tlsext s->session->tlsext_ellipticcurvelist (length=%i) ", s->session->tlsext_ellipticcurvelist_length);
 			sdata = s->session->tlsext_ellipticcurvelist;
 			for (i = 0; i < s->session->tlsext_ellipticcurvelist_length; i++)
 				fprintf(stderr,"%i ",*(sdata++));
 			fprintf(stderr,"\n");
 #endif
 			}
 #endif  
 #ifdef TLSEXT_TYPE_opaque_prf_input
 		else if (type == TLSEXT_TYPE_opaque_prf_input &&
 	             s->version != DTLS1_VERSION)
 			{
 			unsigned char *sdata = data;
 
 			if (size < 2)
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			n2s(sdata, s->s3->client_opaque_prf_input_len);
 			if (s->s3->client_opaque_prf_input_len != size - 2)
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 
 			if (s->s3->client_opaque_prf_input != NULL)  
 				OPENSSL_free(s->s3->client_opaque_prf_input);
 			if (s->s3->client_opaque_prf_input_len == 0)
 				s->s3->client_opaque_prf_input = OPENSSL_malloc(1);  
 			else
 				s->s3->client_opaque_prf_input = BUF_memdup(sdata, s->s3->client_opaque_prf_input_len);
 			if (s->s3->client_opaque_prf_input == NULL)
 				{
 				*al = TLS1_AD_INTERNAL_ERROR;
 				return 0;
 				}
 			}
 #endif
 		else if (type == TLSEXT_TYPE_session_ticket)
 			{
 			if (s->tls_session_ticket_ext_cb &&
 			    !s->tls_session_ticket_ext_cb(s, data, size, s->tls_session_ticket_ext_cb_arg))
 				{
 				*al = TLS1_AD_INTERNAL_ERROR;
 				return 0;
 				}
 			}
 		else if (type == TLSEXT_TYPE_renegotiate)
 			{
 			if(!ssl_parse_clienthello_renegotiate_ext(s, data, size, al))
 				return 0;
 			renegotiate_seen = 1;
 			}
 		else if (type == TLSEXT_TYPE_signature_algorithms)
 			{
 			int dsize;
 			if (sigalg_seen || size < 2) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			sigalg_seen = 1;
 			n2s(data,dsize);
 			size -= 2;
 			if (dsize != size || dsize & 1) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			if (!tls1_process_sigalgs(s, data, dsize))
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 			}
 		else if (type == TLSEXT_TYPE_status_request &&
 		         s->version != DTLS1_VERSION)
 			{
 		
 			if (size < 5) 
 				{
 				*al = SSL_AD_DECODE_ERROR;
 				return 0;
 				}
 
 			s->tlsext_status_type = *data++;
 			size--;
 			if (s->tlsext_status_type == TLSEXT_STATUSTYPE_ocsp)
 				{
 				const unsigned char *sdata;
 				int dsize;
 				 
 				n2s(data,dsize);
 				size -= 2;
 				if (dsize > size  ) 
 					{
 					*al = SSL_AD_DECODE_ERROR;
 					return 0;
 					}
 				while (dsize > 0)
 					{
 					OCSP_RESPID *id;
 					int idsize;
 					if (dsize < 4)
 						{
 						*al = SSL_AD_DECODE_ERROR;
 						return 0;
 						}
 					n2s(data, idsize);
 					dsize -= 2 + idsize;
 					size -= 2 + idsize;
 					if (dsize < 0)
 						{
 						*al = SSL_AD_DECODE_ERROR;
 						return 0;
 						}
 					sdata = data;
 					data += idsize;
 					id = d2i_OCSP_RESPID(NULL,
 								&sdata, idsize);
 					if (!id)
 						{
 						*al = SSL_AD_DECODE_ERROR;
 						return 0;
 						}
 					if (data != sdata)
 						{
 						OCSP_RESPID_free(id);
 						*al = SSL_AD_DECODE_ERROR;
 						return 0;
 						}
 					if (!s->tlsext_ocsp_ids
 						&& !(s->tlsext_ocsp_ids =
 						sk_OCSP_RESPID_new_null()))
 						{
 						OCSP_RESPID_free(id);
 						*al = SSL_AD_INTERNAL_ERROR;
 						return 0;
 						}
 					if (!sk_OCSP_RESPID_push(
 							s->tlsext_ocsp_ids, id))
 						{
 						OCSP_RESPID_free(id);
 						*al = SSL_AD_INTERNAL_ERROR;
 						return 0;
 						}
 					}
 
 				 
 				if (size < 2)
 					{
 					*al = SSL_AD_DECODE_ERROR;
 					return 0;
 					}
 				n2s(data,dsize);
 				size -= 2;
 				if (dsize != size)
 					{
 					*al = SSL_AD_DECODE_ERROR;
 					return 0;
 					}
 				sdata = data;
 				if (dsize > 0)
 					{
 					if (s->tlsext_ocsp_exts)
 						{
 						sk_X509_EXTENSION_pop_free(s->tlsext_ocsp_exts,
 									   X509_EXTENSION_free);
 						}
 
 					s->tlsext_ocsp_exts =
 						d2i_X509_EXTENSIONS(NULL,
 							&sdata, dsize);
 					if (!s->tlsext_ocsp_exts
 						|| (data + dsize != sdata))
 						{
 						*al = SSL_AD_DECODE_ERROR;
 						return 0;
 						}
 					}
 				}
 				 
 				else
 					s->tlsext_status_type = -1;
 			}
 #ifndef OPENSSL_NO_HEARTBEATS
 		else if (type == TLSEXT_TYPE_heartbeat)
 			{
 			switch(data[0])
 				{
 				case 0x01:	 
 							s->tlsext_heartbeat |= SSL_TLSEXT_HB_ENABLED;
 							break;
 				case 0x02:	 
 							s->tlsext_heartbeat |= SSL_TLSEXT_HB_ENABLED;
 							s->tlsext_heartbeat |= SSL_TLSEXT_HB_DONT_SEND_REQUESTS;
 							break;
 				default:	*al = SSL_AD_ILLEGAL_PARAMETER;
 							return 0;
 				}
 			}
 #endif
 #ifndef OPENSSL_NO_NEXTPROTONEG
 		else if (type == TLSEXT_TYPE_next_proto_neg &&
 			 s->s3->tmp.finish_md_len == 0)
 			{
 			 
 			s->s3->next_proto_neg_seen = 1;
 			}
 #endif
  
                  
  #ifndef OPENSSL_NO_SRTP
               else if (type == TLSEXT_TYPE_use_srtp)
//                else if (SSL_IS_DTLS(s) && SSL_get_srtp_profiles(s)
//                         && type == TLSEXT_TYPE_use_srtp)
                         {
                         if(ssl_parse_clienthello_use_srtp_ext(s, data, size,
                                                               al))
 			}
 #endif
 
 		data+=size;
 		}
 				
 	*p = data;
 
 	ri_check:
 
 	 
 
 	if (!renegotiate_seen && s->renegotiate &&
 		!(s->options & SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION))
 		{
 		*al = SSL_AD_HANDSHAKE_FAILURE;
 	 	SSLerr(SSL_F_SSL_PARSE_CLIENTHELLO_TLSEXT,
 				SSL_R_UNSAFE_LEGACY_RENEGOTIATION_DISABLED);
 		return 0;
 		}
 
 	return 1;
 	}