int ssl3_check_cert_and_algorithm(SSL *s)
	{
	int i,idx;
	long alg_k,alg_a;
	EVP_PKEY *pkey=NULL;
	SESS_CERT *sc;
#ifndef OPENSSL_NO_RSA
	RSA *rsa;
#endif
#ifndef OPENSSL_NO_DH
	DH *dh;
#endif

	alg_k=s->s3->tmp.new_cipher->algorithm_mkey;
	alg_a=s->s3->tmp.new_cipher->algorithm_auth;

	 
	if ((alg_a & (SSL_aDH|SSL_aNULL|SSL_aKRB5)) || (alg_k & SSL_kPSK))
		return(1);

	sc=s->session->sess_cert;
	if (sc == NULL)
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,ERR_R_INTERNAL_ERROR);
		goto err;
		}

#ifndef OPENSSL_NO_RSA
	rsa=s->session->sess_cert->peer_rsa_tmp;
#endif
#ifndef OPENSSL_NO_DH
	dh=s->session->sess_cert->peer_dh_tmp;
#endif

	 

	idx=sc->peer_cert_type;
#ifndef OPENSSL_NO_ECDH
	if (idx == SSL_PKEY_ECC)
		{
		if (ssl_check_srvr_ecc_cert_and_alg(sc->peer_pkeys[idx].x509,
		    						s) == 0) 
			{  
			SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_BAD_ECC_CERT);
			goto f_err;
			}
		else 
			{
			return 1;
			}
		}
#endif
	pkey=X509_get_pubkey(sc->peer_pkeys[idx].x509);
	i=X509_certificate_type(sc->peer_pkeys[idx].x509,pkey);
	EVP_PKEY_free(pkey);

	
	 
	if ((alg_a & SSL_aRSA) && !has_bits(i,EVP_PK_RSA|EVP_PKT_SIGN))
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_RSA_SIGNING_CERT);
		goto f_err;
		}
#ifndef OPENSSL_NO_DSA
	else if ((alg_a & SSL_aDSS) && !has_bits(i,EVP_PK_DSA|EVP_PKT_SIGN))
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_DSA_SIGNING_CERT);
		goto f_err;
		}
#endif
#ifndef OPENSSL_NO_RSA
	if ((alg_k & SSL_kRSA) &&
		!(has_bits(i,EVP_PK_RSA|EVP_PKT_ENC) || (rsa != NULL)))
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_RSA_ENCRYPTING_CERT);
		goto f_err;
		}
#endif
#ifndef OPENSSL_NO_DH
	if ((alg_k & SSL_kEDH) &&
		!(has_bits(i,EVP_PK_DH|EVP_PKT_EXCH) || (dh != NULL)))
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_DH_KEY);
		goto f_err;
		}
	else if ((alg_k & SSL_kDHr) && !has_bits(i,EVP_PK_DH|EVP_PKS_RSA))
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_DH_RSA_CERT);
		goto f_err;
		}
#ifndef OPENSSL_NO_DSA
	else if ((alg_k & SSL_kDHd) && !has_bits(i,EVP_PK_DH|EVP_PKS_DSA))
		{
		SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_DH_DSA_CERT);
		goto f_err;
		}
#endif
#endif

	if (SSL_C_IS_EXPORT(s->s3->tmp.new_cipher) && !has_bits(i,EVP_PKT_EXP))
		{
#ifndef OPENSSL_NO_RSA
		if (alg_k & SSL_kRSA)
			{
			if (rsa == NULL
			    || RSA_size(rsa)*8 > SSL_C_EXPORT_PKEYLENGTH(s->s3->tmp.new_cipher))
				{
				SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_EXPORT_TMP_RSA_KEY);
				goto f_err;
				}
			}
		else
#endif
#ifndef OPENSSL_NO_DH
			if (alg_k & (SSL_kEDH|SSL_kDHr|SSL_kDHd))
			    {
			    if (dh == NULL
				|| DH_size(dh)*8 > SSL_C_EXPORT_PKEYLENGTH(s->s3->tmp.new_cipher))
				{
				SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_MISSING_EXPORT_TMP_DH_KEY);
				goto f_err;
				}
			}
		else
#endif
			{
			SSLerr(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM,SSL_R_UNKNOWN_KEY_EXCHANGE_TYPE);
			goto f_err;
			}
		}
	return(1);
f_err:
	ssl3_send_alert(s,SSL3_AL_FATAL,SSL_AD_HANDSHAKE_FAILURE);
err:
	return(0);
	}