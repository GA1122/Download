unsigned char *ssl_add_clienthello_tlsext(SSL *s, unsigned char *buf, unsigned char *limit, int *al)
	{
	int extdatalen=0;
	unsigned char *orig = buf;
	unsigned char *ret = buf;
#ifndef OPENSSL_NO_EC
	 
	int using_ecc = 0;
	if (s->version >= TLS1_VERSION || SSL_IS_DTLS(s))
		{
		int i;
		unsigned long alg_k, alg_a;
		STACK_OF(SSL_CIPHER) *cipher_stack = SSL_get_ciphers(s);

		for (i = 0; i < sk_SSL_CIPHER_num(cipher_stack); i++)
			{
			SSL_CIPHER *c = sk_SSL_CIPHER_value(cipher_stack, i);

			alg_k = c->algorithm_mkey;
			alg_a = c->algorithm_auth;
			if ((alg_k & (SSL_kECDHE|SSL_kECDHr|SSL_kECDHe)
				|| (alg_a & SSL_aECDSA)))
				{
				using_ecc = 1;
				break;
				}
			}
		}
#endif

	 
	if (s->client_version == SSL3_VERSION
					&& !s->s3->send_connection_binding)
		return orig;

	ret+=2;

	if (ret>=limit) return NULL;  

 	if (s->tlsext_hostname != NULL)
		{ 
		 
		unsigned long size_str;
		long lenmax; 

		 
		   
		if ((lenmax = limit - ret - 9) < 0 
		    || (size_str = strlen(s->tlsext_hostname)) > (unsigned long)lenmax) 
			return NULL;
			
		 
		s2n(TLSEXT_TYPE_server_name,ret); 
		s2n(size_str+5,ret);
		
		 
		s2n(size_str+3,ret);
	
		 
		*(ret++) = (unsigned char) TLSEXT_NAMETYPE_host_name;
		s2n(size_str,ret);
		memcpy(ret, s->tlsext_hostname, size_str);
		ret+=size_str;
		}

         
        if (s->renegotiate)
          {
          int el;
          
          if(!ssl_add_clienthello_renegotiate_ext(s, 0, &el, 0))
              {
              SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
              return NULL;
              }

          if((limit - ret - 4 - el) < 0) return NULL;
          
          s2n(TLSEXT_TYPE_renegotiate,ret);
          s2n(el,ret);

          if(!ssl_add_clienthello_renegotiate_ext(s, ret, &el, el))
              {
              SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
              return NULL;
              }

          ret += el;
        }

#ifndef OPENSSL_NO_SRP
	 
	if (s->srp_ctx.login != NULL)
		{  

		int login_len = strlen(s->srp_ctx.login);	
		if (login_len > 255 || login_len == 0)
			{
			SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
			return NULL;
			} 

		 
		if ((limit - ret - 5 - login_len) < 0) return NULL; 

		 
		s2n(TLSEXT_TYPE_srp,ret);
		s2n(login_len+1,ret);
		(*ret++) = (unsigned char) login_len;
		memcpy(ret, s->srp_ctx.login, login_len);
		ret+=login_len;
		}
#endif

#ifndef OPENSSL_NO_EC
	if (using_ecc)
		{
		 
		long lenmax; 
		const unsigned char *plist;
		size_t plistlen;
		size_t i;
		unsigned char *etmp;

		tls1_get_formatlist(s, &plist, &plistlen);

		if ((lenmax = limit - ret - 5) < 0) return NULL; 
		if (plistlen > (size_t)lenmax) return NULL;
		if (plistlen > 255)
			{
			SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
			return NULL;
			}
		
		s2n(TLSEXT_TYPE_ec_point_formats,ret);
		s2n(plistlen + 1,ret);
		*(ret++) = (unsigned char)plistlen ;
		memcpy(ret, plist, plistlen);
		ret+=plistlen;

		 
		plist = s->tlsext_ellipticcurvelist;
		tls1_get_curvelist(s, 0, &plist, &plistlen);

		if ((lenmax = limit - ret - 6) < 0) return NULL; 
		if (plistlen > (size_t)lenmax) return NULL;
		if (plistlen > 65532)
			{
			SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
			return NULL;
			}

		
		s2n(TLSEXT_TYPE_elliptic_curves,ret);
		etmp = ret + 4;
		 
		for (i = 0; i < plistlen; i += 2, plist += 2)
			{
			if (tls_curve_allowed(s, plist, SSL_SECOP_CURVE_SUPPORTED))
				{
				*etmp++ = plist[0];
				*etmp++ = plist[1];
				}
			}

		plistlen = etmp - ret - 4;

		 
		s2n(plistlen + 2, ret);
		s2n(plistlen, ret);
		ret+=plistlen;
		}
#endif  

	if (tls_use_ticket(s))
		{
		int ticklen;
		if (!s->new_session && s->session && s->session->tlsext_tick)
			ticklen = s->session->tlsext_ticklen;
		else if (s->session && s->tlsext_session_ticket &&
			 s->tlsext_session_ticket->data)
			{
			ticklen = s->tlsext_session_ticket->length;
			s->session->tlsext_tick = OPENSSL_malloc(ticklen);
			if (!s->session->tlsext_tick)
				return NULL;
			memcpy(s->session->tlsext_tick,
			       s->tlsext_session_ticket->data,
			       ticklen);
			s->session->tlsext_ticklen = ticklen;
			}
		else
			ticklen = 0;
		if (ticklen == 0 && s->tlsext_session_ticket &&
		    s->tlsext_session_ticket->data == NULL)
			goto skip_ext;
		 
		if ((long)(limit - ret - 4 - ticklen) < 0) return NULL;
		s2n(TLSEXT_TYPE_session_ticket,ret); 
		s2n(ticklen,ret);
		if (ticklen)
			{
			memcpy(ret, s->session->tlsext_tick, ticklen);
			ret += ticklen;
			}
		}
		skip_ext:

	if (SSL_USE_SIGALGS(s))
		{
		size_t salglen;
		const unsigned char *salg;
		unsigned char *etmp;
		salglen = tls12_get_psigalgs(s, &salg);
		if ((size_t)(limit - ret) < salglen + 6)
			return NULL; 
		s2n(TLSEXT_TYPE_signature_algorithms,ret);
		etmp = ret;
		 
		ret += 4;
		salglen = tls12_copy_sigalgs(s, ret, salg, salglen);
		 
		s2n(salglen + 2, etmp);
		s2n(salglen, etmp);
		ret += salglen;
		}

#ifdef TLSEXT_TYPE_opaque_prf_input
	if (s->s3->client_opaque_prf_input != NULL)
		{
		size_t col = s->s3->client_opaque_prf_input_len;
		
		if ((long)(limit - ret - 6 - col) < 0)
			return NULL;
		if (col > 0xFFFD)  
			return NULL;

		s2n(TLSEXT_TYPE_opaque_prf_input, ret); 
		s2n(col + 2, ret);
		s2n(col, ret);
		memcpy(ret, s->s3->client_opaque_prf_input, col);
		ret += col;
		}
#endif

	if (s->tlsext_status_type == TLSEXT_STATUSTYPE_ocsp)
		{
		int i;
		long extlen, idlen, itmp;
		OCSP_RESPID *id;

		idlen = 0;
		for (i = 0; i < sk_OCSP_RESPID_num(s->tlsext_ocsp_ids); i++)
			{
			id = sk_OCSP_RESPID_value(s->tlsext_ocsp_ids, i);
			itmp = i2d_OCSP_RESPID(id, NULL);
			if (itmp <= 0)
				return NULL;
			idlen += itmp + 2;
			}

		if (s->tlsext_ocsp_exts)
			{
			extlen = i2d_X509_EXTENSIONS(s->tlsext_ocsp_exts, NULL);
			if (extlen < 0)
				return NULL;
			}
		else
			extlen = 0;
			
		if ((long)(limit - ret - 7 - extlen - idlen) < 0) return NULL;
		s2n(TLSEXT_TYPE_status_request, ret);
		if (extlen + idlen > 0xFFF0)
			return NULL;
		s2n(extlen + idlen + 5, ret);
		*(ret++) = TLSEXT_STATUSTYPE_ocsp;
		s2n(idlen, ret);
		for (i = 0; i < sk_OCSP_RESPID_num(s->tlsext_ocsp_ids); i++)
			{
			 
			unsigned char *q = ret;
			id = sk_OCSP_RESPID_value(s->tlsext_ocsp_ids, i);
			 
			ret += 2;
			itmp = i2d_OCSP_RESPID(id, &ret);
			 
			s2n(itmp, q);
			}
		s2n(extlen, ret);
		if (extlen > 0)
			i2d_X509_EXTENSIONS(s->tlsext_ocsp_exts, &ret);
		}

#ifndef OPENSSL_NO_HEARTBEATS
	 
	if ((limit - ret - 4 - 1) < 0)
		return NULL;
	s2n(TLSEXT_TYPE_heartbeat,ret);
	s2n(1,ret);
	 
	if (s->tlsext_heartbeat & SSL_TLSEXT_HB_DONT_RECV_REQUESTS)
		*(ret++) = SSL_TLSEXT_HB_DONT_SEND_REQUESTS;
	else
		*(ret++) = SSL_TLSEXT_HB_ENABLED;
#endif

#ifndef OPENSSL_NO_NEXTPROTONEG
	if (s->ctx->next_proto_select_cb && !s->s3->tmp.finish_md_len)
		{
		 
		if (limit - ret - 4 < 0)
			return NULL;
		s2n(TLSEXT_TYPE_next_proto_neg,ret);
		s2n(0,ret);
		}
#endif

	if (s->alpn_client_proto_list && !s->s3->tmp.finish_md_len)
		{
		if ((size_t)(limit - ret) < 6 + s->alpn_client_proto_list_len)
			return NULL;
		s2n(TLSEXT_TYPE_application_layer_protocol_negotiation,ret);
		s2n(2 + s->alpn_client_proto_list_len,ret);
		s2n(s->alpn_client_proto_list_len,ret);
		memcpy(ret, s->alpn_client_proto_list,
		       s->alpn_client_proto_list_len);
		ret += s->alpn_client_proto_list_len;
		}

        if(SSL_get_srtp_profiles(s))
                {
                int el;

                ssl_add_clienthello_use_srtp_ext(s, 0, &el, 0);
                
                if((limit - ret - 4 - el) < 0) return NULL;

                s2n(TLSEXT_TYPE_use_srtp,ret);
                s2n(el,ret);

                if(ssl_add_clienthello_use_srtp_ext(s, ret, &el, el))
			{
			SSLerr(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT, ERR_R_INTERNAL_ERROR);
			return NULL;
			}
                ret += el;
                }

	 
	if (s->ctx->custom_cli_ext_records_count)
		{
		size_t i;
		custom_cli_ext_record* record;

		for (i = 0; i < s->ctx->custom_cli_ext_records_count; i++)
			{
			const unsigned char* out = NULL;
			unsigned short outlen = 0;

			record = &s->ctx->custom_cli_ext_records[i];
			  
			 
			if (record->fn1)
				{
				int cb_retval = 0;
				cb_retval = record->fn1(s, record->ext_type,
							&out, &outlen, al,
							record->arg);
				if (cb_retval == 0)
					return NULL;  
				if (cb_retval == -1)
					continue;  
				}
			if (limit < ret + 4 + outlen)
				return NULL;
			s2n(record->ext_type, ret);
			s2n(outlen, ret);
			memcpy(ret, out, outlen);
			ret += outlen;
			}
		}
#ifdef TLSEXT_TYPE_encrypt_then_mac
	s2n(TLSEXT_TYPE_encrypt_then_mac,ret);
	s2n(0,ret);
#endif

	 
	if (s->options & SSL_OP_TLSEXT_PADDING)
		{
		int hlen = ret - (unsigned char *)s->init_buf->data;
		 
		if (s->state == SSL23_ST_CW_CLNT_HELLO_A)
			hlen -= 5;
		if (hlen > 0xff && hlen < 0x200)
			{
			hlen = 0x200 - hlen;
			if (hlen >= 4)
				hlen -= 4;
			else
				hlen = 0;

			s2n(TLSEXT_TYPE_padding, ret);
			s2n(hlen, ret);
			memset(ret, 0, hlen);
			ret += hlen;
			}
		}

	if ((extdatalen = ret-orig-2)== 0) 
		return orig;

	s2n(extdatalen, orig);
	return ret;
	}
