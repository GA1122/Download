long Condor_Auth_SSL :: post_connection_check(SSL *ssl, int   )
{
	X509      *cert;
		 
    ouch("post_connection_check.\n");
 
     
    cert = SSL_get_peer_certificate(ssl);
    if( cert == NULL ) {
        dprintf(D_SECURITY,"SSL_get_peer_certificate returned null.\n" );
        goto err_occured;
    }
    dprintf(D_SECURITY,"SSL_get_peer_certificate returned data.\n" );

     
    ouch("Returning SSL_get_verify_result.\n");
    
    X509_free(cert);
    return SSL_get_verify_result(ssl);
 
err_occured:
    if (cert)
        X509_free(cert);
    return X509_V_ERR_APPLICATION_VERIFICATION;
}
