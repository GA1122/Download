int ssl3_send_client_key_exchange(SSL *s)
	{
	unsigned char *p;
	int n;
	unsigned long alg_k;
#ifndef OPENSSL_NO_RSA
	unsigned char *q;
	EVP_PKEY *pkey=NULL;
#endif
#ifndef OPENSSL_NO_KRB5
	KSSL_ERR kssl_err;
#endif  
#ifndef OPENSSL_NO_ECDH
	EC_KEY *clnt_ecdh = NULL;
	const EC_POINT *srvr_ecpoint = NULL;
	EVP_PKEY *srvr_pub_pkey = NULL;
	unsigned char *encodedPoint = NULL;
	int encoded_pt_len = 0;
	BN_CTX * bn_ctx = NULL;
#endif

	if (s->state == SSL3_ST_CW_KEY_EXCH_A)
		{
		p = ssl_handshake_start(s);

		alg_k=s->s3->tmp.new_cipher->algorithm_mkey;

		 
		if (0) {}
#ifndef OPENSSL_NO_RSA
		else if (alg_k & SSL_kRSA)
			{
			RSA *rsa;
			unsigned char tmp_buf[SSL_MAX_MASTER_KEY_LENGTH];

			if (s->session->sess_cert == NULL)
				{
				 
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_INTERNAL_ERROR);
				goto err;
				}

			if (s->session->sess_cert->peer_rsa_tmp != NULL)
				rsa=s->session->sess_cert->peer_rsa_tmp;
			else
				{
				pkey=X509_get_pubkey(s->session->sess_cert->peer_pkeys[SSL_PKEY_RSA_ENC].x509);
				if ((pkey == NULL) ||
					(pkey->type != EVP_PKEY_RSA) ||
					(pkey->pkey.rsa == NULL))
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_INTERNAL_ERROR);
					goto err;
					}
				rsa=pkey->pkey.rsa;
				EVP_PKEY_free(pkey);
				}
				
			tmp_buf[0]=s->client_version>>8;
			tmp_buf[1]=s->client_version&0xff;
			if (RAND_bytes(&(tmp_buf[2]),sizeof tmp_buf-2) <= 0)
					goto err;

			s->session->master_key_length=sizeof tmp_buf;

			q=p;
			 
			if (s->version > SSL3_VERSION)
				p+=2;
			n=RSA_public_encrypt(sizeof tmp_buf,
				tmp_buf,p,rsa,RSA_PKCS1_PADDING);
#ifdef PKCS1_CHECK
			if (s->options & SSL_OP_PKCS1_CHECK_1) p[1]++;
			if (s->options & SSL_OP_PKCS1_CHECK_2) tmp_buf[0]=0x70;
#endif
			if (n <= 0)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,SSL_R_BAD_RSA_ENCRYPT);
				goto err;
				}

			 
			if (s->version > SSL3_VERSION)
				{
				s2n(n,q);
				n+=2;
				}

			s->session->master_key_length=
				s->method->ssl3_enc->generate_master_secret(s,
					s->session->master_key,
					tmp_buf,sizeof tmp_buf);
			OPENSSL_cleanse(tmp_buf,sizeof tmp_buf);
			}
#endif
#ifndef OPENSSL_NO_KRB5
		else if (alg_k & SSL_kKRB5)
			{
			krb5_error_code	krb5rc;
			KSSL_CTX	*kssl_ctx = s->kssl_ctx;
			 
			krb5_data	*enc_ticket;
			krb5_data	authenticator, *authp = NULL;
			EVP_CIPHER_CTX	ciph_ctx;
			const EVP_CIPHER *enc = NULL;
			unsigned char	iv[EVP_MAX_IV_LENGTH];
			unsigned char	tmp_buf[SSL_MAX_MASTER_KEY_LENGTH];
			unsigned char	epms[SSL_MAX_MASTER_KEY_LENGTH 
						+ EVP_MAX_IV_LENGTH];
			int 		padl, outl = sizeof(epms);

			EVP_CIPHER_CTX_init(&ciph_ctx);

#ifdef KSSL_DEBUG
			fprintf(stderr,"ssl3_send_client_key_exchange(%lx & %lx)\n",
				alg_k, SSL_kKRB5);
#endif	 

			authp = NULL;
#ifdef KRB5SENDAUTH
			if (KRB5SENDAUTH)  authp = &authenticator;
#endif	 

			krb5rc = kssl_cget_tkt(kssl_ctx, &enc_ticket, authp,
				&kssl_err);
			enc = kssl_map_enc(kssl_ctx->enctype);
			if (enc == NULL)
			    goto err;
#ifdef KSSL_DEBUG
			{
			fprintf(stderr,"kssl_cget_tkt rtn %d\n", krb5rc);
			if (krb5rc && kssl_err.text)
			  fprintf(stderr,"kssl_cget_tkt kssl_err=%s\n", kssl_err.text);
			}
#endif	 

			if (krb5rc)
				{
				ssl3_send_alert(s,SSL3_AL_FATAL,
						SSL_AD_HANDSHAKE_FAILURE);
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
						kssl_err.reason);
				goto err;
				}

			 

			 
			s2n(enc_ticket->length,p);
			memcpy(p, enc_ticket->data, enc_ticket->length);
			p+= enc_ticket->length;
			n = enc_ticket->length + 2;

			 
			if (authp  &&  authp->length)  
				{
				s2n(authp->length,p);
				memcpy(p, authp->data, authp->length);
				p+= authp->length;
				n+= authp->length + 2;
				
				free(authp->data);
				authp->data = NULL;
				authp->length = 0;
				}
			else
				{
				s2n(0,p); 
				n+=2;
				}
 
			    tmp_buf[0]=s->client_version>>8;
			    tmp_buf[1]=s->client_version&0xff;
			    if (RAND_bytes(&(tmp_buf[2]),sizeof tmp_buf-2) <= 0)
				goto err;

			 

			memset(iv, 0, sizeof iv);   
			EVP_EncryptInit_ex(&ciph_ctx,enc, NULL,
				kssl_ctx->key,iv);
			EVP_EncryptUpdate(&ciph_ctx,epms,&outl,tmp_buf,
				sizeof tmp_buf);
			EVP_EncryptFinal_ex(&ciph_ctx,&(epms[outl]),&padl);
			outl += padl;
			if (outl > (int)sizeof epms)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE, ERR_R_INTERNAL_ERROR);
				goto err;
				}
			EVP_CIPHER_CTX_cleanup(&ciph_ctx);

			 
			s2n(outl,p);
			memcpy(p, epms, outl);
			p+=outl;
			n+=outl + 2;

			s->session->master_key_length=
				s->method->ssl3_enc->generate_master_secret(s,
					s->session->master_key,
					tmp_buf, sizeof tmp_buf);

			OPENSSL_cleanse(tmp_buf, sizeof tmp_buf);
			OPENSSL_cleanse(epms, outl);
			}
#endif
#ifndef OPENSSL_NO_DH
		else if (alg_k & (SSL_kDHE|SSL_kDHr|SSL_kDHd))
			{
			DH *dh_srvr,*dh_clnt;
			SESS_CERT *scert = s->session->sess_cert;

			if (scert == NULL) 
				{
				ssl3_send_alert(s,SSL3_AL_FATAL,SSL_AD_UNEXPECTED_MESSAGE);
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,SSL_R_UNEXPECTED_MESSAGE);
				goto err;
				}

			if (scert->peer_dh_tmp != NULL)
				dh_srvr=scert->peer_dh_tmp;
			else
				{
				 
				int idx = scert->peer_cert_type;
				EVP_PKEY *spkey = NULL;
				dh_srvr = NULL;
				if (idx >= 0)
					spkey = X509_get_pubkey(
						scert->peer_pkeys[idx].x509);
				if (spkey)
					{
					dh_srvr = EVP_PKEY_get1_DH(spkey);
					EVP_PKEY_free(spkey);
					}
				if (dh_srvr == NULL)
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					    ERR_R_INTERNAL_ERROR);
					goto err;
					}
				}
			if (s->s3->flags & TLS1_FLAGS_SKIP_CERT_VERIFY)
				{
				 
				EVP_PKEY *clkey = s->cert->key->privatekey;
				dh_clnt = NULL;
				if (clkey)
					dh_clnt = EVP_PKEY_get1_DH(clkey);
				if (dh_clnt == NULL)
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					    ERR_R_INTERNAL_ERROR);
					goto err;
					}
				}
			else
				{
				 
				if ((dh_clnt=DHparams_dup(dh_srvr)) == NULL)
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_DH_LIB);
					goto err;
					}
				if (!DH_generate_key(dh_clnt))
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_DH_LIB);
					DH_free(dh_clnt);
					goto err;
					}
				}

			 

			n=DH_compute_key(p,dh_srvr->pub_key,dh_clnt);
			if (scert->peer_dh_tmp == NULL)
				DH_free(dh_srvr);

			if (n <= 0)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_DH_LIB);
				DH_free(dh_clnt);
				goto err;
				}

			 
			s->session->master_key_length=
				s->method->ssl3_enc->generate_master_secret(s,
					s->session->master_key,p,n);
			 
			memset(p,0,n);

			if (s->s3->flags & TLS1_FLAGS_SKIP_CERT_VERIFY)
				n = 0;
			else
				{
				 
				n=BN_num_bytes(dh_clnt->pub_key);
				s2n(n,p);
				BN_bn2bin(dh_clnt->pub_key,p);
				n+=2;
				}

			DH_free(dh_clnt);

			 
			}
#endif

#ifndef OPENSSL_NO_ECDH 
		else if (alg_k & (SSL_kECDHE|SSL_kECDHr|SSL_kECDHe))
			{
			const EC_GROUP *srvr_group = NULL;
			EC_KEY *tkey;
			int ecdh_clnt_cert = 0;
			int field_size = 0;

			if (s->session->sess_cert == NULL) 
				{
				ssl3_send_alert(s,SSL3_AL_FATAL,SSL_AD_UNEXPECTED_MESSAGE);
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,SSL_R_UNEXPECTED_MESSAGE);
				goto err;
				}

			 
			if ((alg_k & (SSL_kECDHr|SSL_kECDHe)) && (s->cert != NULL)) 
				{
				 
				}

			if (s->session->sess_cert->peer_ecdh_tmp != NULL)
				{
				tkey = s->session->sess_cert->peer_ecdh_tmp;
				}
			else
				{
				 
				srvr_pub_pkey = X509_get_pubkey(s->session-> \
				    sess_cert->peer_pkeys[SSL_PKEY_ECC].x509);
				if ((srvr_pub_pkey == NULL) ||
				    (srvr_pub_pkey->type != EVP_PKEY_EC) ||
				    (srvr_pub_pkey->pkey.ec == NULL))
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					    ERR_R_INTERNAL_ERROR);
					goto err;
					}

				tkey = srvr_pub_pkey->pkey.ec;
				}

			srvr_group   = EC_KEY_get0_group(tkey);
			srvr_ecpoint = EC_KEY_get0_public_key(tkey);

			if ((srvr_group == NULL) || (srvr_ecpoint == NULL))
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
				    ERR_R_INTERNAL_ERROR);
				goto err;
				}

			if ((clnt_ecdh=EC_KEY_new()) == NULL) 
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_MALLOC_FAILURE);
				goto err;
				}

			if (!EC_KEY_set_group(clnt_ecdh, srvr_group))
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_EC_LIB);
				goto err;
				}
			if (ecdh_clnt_cert) 
				{ 
				 
				const BIGNUM *priv_key;
				tkey = s->cert->key->privatekey->pkey.ec;
				priv_key = EC_KEY_get0_private_key(tkey);
				if (priv_key == NULL)
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_MALLOC_FAILURE);
					goto err;
					}
				if (!EC_KEY_set_private_key(clnt_ecdh, priv_key))
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_EC_LIB);
					goto err;
					}
				}
			else 
				{
				 
				if (!(EC_KEY_generate_key(clnt_ecdh)))
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE, ERR_R_ECDH_LIB);
					goto err;
					}
				}

			 

			field_size = EC_GROUP_get_degree(srvr_group);
			if (field_size <= 0)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE, 
				       ERR_R_ECDH_LIB);
				goto err;
				}
			n=ECDH_compute_key(p, (field_size+7)/8, srvr_ecpoint, clnt_ecdh, NULL);
			if (n <= 0)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE, 
				       ERR_R_ECDH_LIB);
				goto err;
				}

			 
			s->session->master_key_length = s->method->ssl3_enc \
			    -> generate_master_secret(s, 
				s->session->master_key,
				p, n);

			memset(p, 0, n);  

			if (ecdh_clnt_cert) 
				{
				 
				n = 0;
				}
			else 
				{
				 
				encoded_pt_len = 
				    EC_POINT_point2oct(srvr_group, 
					EC_KEY_get0_public_key(clnt_ecdh), 
					POINT_CONVERSION_UNCOMPRESSED, 
					NULL, 0, NULL);

				encodedPoint = (unsigned char *) 
				    OPENSSL_malloc(encoded_pt_len * 
					sizeof(unsigned char)); 
				bn_ctx = BN_CTX_new();
				if ((encodedPoint == NULL) || 
				    (bn_ctx == NULL)) 
					{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_MALLOC_FAILURE);
					goto err;
					}

				 
				n = EC_POINT_point2oct(srvr_group, 
				    EC_KEY_get0_public_key(clnt_ecdh), 
				    POINT_CONVERSION_UNCOMPRESSED, 
				    encodedPoint, encoded_pt_len, bn_ctx);

				*p = n;  
				 
				p += 1; 
				 
				memcpy((unsigned char *)p, encodedPoint, n);
				 
				n += 1; 
				}

			 
			BN_CTX_free(bn_ctx);
			if (encodedPoint != NULL) OPENSSL_free(encodedPoint);
			if (clnt_ecdh != NULL) 
				 EC_KEY_free(clnt_ecdh);
			EVP_PKEY_free(srvr_pub_pkey);
			}
#endif  
		else if (alg_k & SSL_kGOST) 
			{
			 
			EVP_PKEY_CTX *pkey_ctx;
			X509 *peer_cert; 
			size_t msglen;
			unsigned int md_len;
			int keytype;
			unsigned char premaster_secret[32],shared_ukm[32], tmp[256];
			EVP_MD_CTX *ukm_hash;
			EVP_PKEY *pub_key;

			 
			peer_cert=s->session->sess_cert->peer_pkeys[(keytype=SSL_PKEY_GOST01)].x509;
			if (!peer_cert) 
				peer_cert=s->session->sess_cert->peer_pkeys[(keytype=SSL_PKEY_GOST94)].x509;
			if (!peer_cert)		{
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,SSL_R_NO_GOST_CERTIFICATE_SENT_BY_PEER);
					goto err;
				}	
				
			pkey_ctx=EVP_PKEY_CTX_new(pub_key=X509_get_pubkey(peer_cert),NULL);
			 

			  
					
			EVP_PKEY_encrypt_init(pkey_ctx);
			   	
		    RAND_bytes(premaster_secret,32);
			 
			if (s->s3->tmp.cert_req && s->cert->key->privatekey) {
				if (EVP_PKEY_derive_set_peer(pkey_ctx,s->cert->key->privatekey) <=0) {
					 
					ERR_clear_error();
				}
			}			
			 
			ukm_hash = EVP_MD_CTX_create();
			EVP_DigestInit(ukm_hash,EVP_get_digestbynid(NID_id_GostR3411_94));
			EVP_DigestUpdate(ukm_hash,s->s3->client_random,SSL3_RANDOM_SIZE);
			EVP_DigestUpdate(ukm_hash,s->s3->server_random,SSL3_RANDOM_SIZE);
			EVP_DigestFinal_ex(ukm_hash, shared_ukm, &md_len);
			EVP_MD_CTX_destroy(ukm_hash);
			if (EVP_PKEY_CTX_ctrl(pkey_ctx,-1,EVP_PKEY_OP_ENCRYPT,EVP_PKEY_CTRL_SET_IV,
				8,shared_ukm)<0) {
					SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
						SSL_R_LIBRARY_BUG);
					goto err;
				}	
			 
			 
			*(p++)=V_ASN1_SEQUENCE | V_ASN1_CONSTRUCTED;
			msglen=255;
			if (EVP_PKEY_encrypt(pkey_ctx,tmp,&msglen,premaster_secret,32)<0) {
			SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					SSL_R_LIBRARY_BUG);
				goto err;
			}
			if (msglen >= 0x80)
				{
				*(p++)=0x81;
				*(p++)= msglen & 0xff;
				n=msglen+3;
				}
			else
				{
				*(p++)= msglen & 0xff;
				n=msglen+2;
				}
			memcpy(p, tmp, msglen);
			 
			if (EVP_PKEY_CTX_ctrl(pkey_ctx, -1, -1, EVP_PKEY_CTRL_PEER_KEY, 2, NULL) > 0)
				{
				 
				s->s3->flags |= TLS1_FLAGS_SKIP_CERT_VERIFY;
				}
			EVP_PKEY_CTX_free(pkey_ctx);
			s->session->master_key_length=
				s->method->ssl3_enc->generate_master_secret(s,
					s->session->master_key,premaster_secret,32);
			EVP_PKEY_free(pub_key);

			}
#ifndef OPENSSL_NO_SRP
		else if (alg_k & SSL_kSRP)
			{
			if (s->srp_ctx.A != NULL)
				{
				 
				n=BN_num_bytes(s->srp_ctx.A);
				s2n(n,p);
				BN_bn2bin(s->srp_ctx.A,p);
				n+=2;
				}
			else
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_INTERNAL_ERROR);
				goto err;
				}
			if (s->session->srp_username != NULL)
				OPENSSL_free(s->session->srp_username);
			s->session->srp_username = BUF_strdup(s->srp_ctx.login);
			if (s->session->srp_username == NULL)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					ERR_R_MALLOC_FAILURE);
				goto err;
				}

			if ((s->session->master_key_length = SRP_generate_client_master_secret(s,s->session->master_key))<0)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,ERR_R_INTERNAL_ERROR);
				goto err;
				}
			}
#endif
#ifndef OPENSSL_NO_PSK
		else if (alg_k & SSL_kPSK)
			{
			 
			char identity[PSK_MAX_IDENTITY_LEN + 2];
			size_t identity_len;
			unsigned char *t = NULL;
			unsigned char psk_or_pre_ms[PSK_MAX_PSK_LEN*2+4];
			unsigned int pre_ms_len = 0, psk_len = 0;
			int psk_err = 1;

			n = 0;
			if (s->psk_client_callback == NULL)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					SSL_R_PSK_NO_CLIENT_CB);
				goto err;
				}

			memset(identity, 0, sizeof(identity));
			psk_len = s->psk_client_callback(s, s->ctx->psk_identity_hint,
				identity, sizeof(identity) - 1,
				psk_or_pre_ms, sizeof(psk_or_pre_ms));
			if (psk_len > PSK_MAX_PSK_LEN)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					ERR_R_INTERNAL_ERROR);
				goto psk_err;
				}
			else if (psk_len == 0)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					SSL_R_PSK_IDENTITY_NOT_FOUND);
				goto psk_err;
				}
			identity[PSK_MAX_IDENTITY_LEN + 1] = '\0';
			identity_len = strlen(identity);
			if (identity_len > PSK_MAX_IDENTITY_LEN)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					ERR_R_INTERNAL_ERROR);
				goto psk_err;
				}
			 
			pre_ms_len = 2+psk_len+2+psk_len;
			t = psk_or_pre_ms;
			memmove(psk_or_pre_ms+psk_len+4, psk_or_pre_ms, psk_len);
			s2n(psk_len, t);
			memset(t, 0, psk_len);
			t+=psk_len;
			s2n(psk_len, t);

			if (s->session->psk_identity_hint != NULL)
				OPENSSL_free(s->session->psk_identity_hint);
			s->session->psk_identity_hint = BUF_strdup(s->ctx->psk_identity_hint);
			if (s->ctx->psk_identity_hint != NULL &&
				s->session->psk_identity_hint == NULL)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					ERR_R_MALLOC_FAILURE);
				goto psk_err;
				}

			if (s->session->psk_identity != NULL)
				OPENSSL_free(s->session->psk_identity);
			s->session->psk_identity = BUF_strdup(identity);
			if (s->session->psk_identity == NULL)
				{
				SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
					ERR_R_MALLOC_FAILURE);
				goto psk_err;
				}

			s->session->master_key_length =
				s->method->ssl3_enc->generate_master_secret(s,
					s->session->master_key,
					psk_or_pre_ms, pre_ms_len);
			s2n(identity_len, p);
			memcpy(p, identity, identity_len);
			n = 2 + identity_len;
			psk_err = 0;
		psk_err:
			OPENSSL_cleanse(identity, sizeof(identity));
			OPENSSL_cleanse(psk_or_pre_ms, sizeof(psk_or_pre_ms));
			if (psk_err != 0)
				{
				ssl3_send_alert(s, SSL3_AL_FATAL, SSL_AD_HANDSHAKE_FAILURE);
				goto err;
				}
			}
#endif
		else
			{
			ssl3_send_alert(s, SSL3_AL_FATAL,
			    SSL_AD_HANDSHAKE_FAILURE);
			SSLerr(SSL_F_SSL3_SEND_CLIENT_KEY_EXCHANGE,
			    ERR_R_INTERNAL_ERROR);
			goto err;
			}

		ssl_set_handshake_header(s, SSL3_MT_CLIENT_KEY_EXCHANGE, n);
		s->state=SSL3_ST_CW_KEY_EXCH_B;
		}

	 
	return ssl_do_write(s);
err:
#ifndef OPENSSL_NO_ECDH
	BN_CTX_free(bn_ctx);
	if (encodedPoint != NULL) OPENSSL_free(encodedPoint);
	if (clnt_ecdh != NULL) 
		EC_KEY_free(clnt_ecdh);
	EVP_PKEY_free(srvr_pub_pkey);
#endif
	return(-1);
	}
