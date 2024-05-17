_SSL_set_verify (SSL_CTX *ctx, void *verify_callback, char *cacert)
{
	if (!SSL_CTX_set_default_verify_paths (ctx))
	{
		__SSL_fill_err_buf ("SSL_CTX_set_default_verify_paths");
		return (err_buf);
	}
 
	SSL_CTX_set_verify (ctx, SSL_VERIFY_PEER, verify_callback);

	return (NULL);
}
