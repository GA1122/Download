int tls1_cert_verify_mac(SSL *s, int md_nid, unsigned char *out)
	{
	unsigned int ret;
	EVP_MD_CTX ctx, *d=NULL;
	int i;

	if (s->s3->handshake_buffer) 
		if (!ssl3_digest_cached_records(s))
			return 0;

	for (i=0;i<SSL_MAX_DIGEST;i++) 
		{
		  if (s->s3->handshake_dgst[i]&&EVP_MD_CTX_type(s->s3->handshake_dgst[i])==md_nid) 
		  	{
		  	d=s->s3->handshake_dgst[i];
			break;
			}
		}
	if (!d) {
		SSLerr(SSL_F_TLS1_CERT_VERIFY_MAC,SSL_R_NO_REQUIRED_DIGEST);
		return 0;
	}	

	EVP_MD_CTX_init(&ctx);
	EVP_MD_CTX_copy_ex(&ctx,d);
	EVP_DigestFinal_ex(&ctx,out,&ret);
	EVP_MD_CTX_cleanup(&ctx);
	return((int)ret);
	}
