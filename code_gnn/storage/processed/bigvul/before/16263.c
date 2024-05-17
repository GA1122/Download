SSL_CTX *Condor_Auth_SSL :: setup_ssl_ctx( bool is_server )
{
    SSL_CTX *ctx       = NULL;
    char *cafile       = NULL;
    char *cadir        = NULL;
    char *certfile     = NULL;
    char *keyfile      = NULL;
    char *cipherlist   = NULL;
    priv_state priv;

	if( is_server ) {
		cafile     = param( AUTH_SSL_SERVER_CAFILE_STR );
		cadir      = param( AUTH_SSL_SERVER_CADIR_STR );
		certfile   = param( AUTH_SSL_SERVER_CERTFILE_STR );
		keyfile    = param( AUTH_SSL_SERVER_KEYFILE_STR );
	} else {
		cafile     = param( AUTH_SSL_CLIENT_CAFILE_STR );
		cadir      = param( AUTH_SSL_CLIENT_CADIR_STR );
		certfile   = param( AUTH_SSL_CLIENT_CERTFILE_STR );
		keyfile    = param( AUTH_SSL_CLIENT_KEYFILE_STR );
	}		
	cipherlist = param( AUTH_SSL_CIPHERLIST_STR );
    if( cipherlist == NULL ) {
		cipherlist = strdup(AUTH_SSL_DEFAULT_CIPHERLIST);
    }
    if( !certfile || !keyfile ) {
        ouch( "Please specify path to server certificate and key\n" );
        dprintf(D_SECURITY, "in config file : '%s' and '%s'.\n",
                AUTH_SSL_SERVER_CERTFILE_STR, AUTH_SSL_SERVER_KEYFILE_STR );
		ctx = NULL;
        goto setup_server_ctx_err;
    }
    if(cafile)     dprintf( D_SECURITY, "CAFILE:     '%s'\n", cafile     );
    if(cadir)      dprintf( D_SECURITY, "CADIR:      '%s'\n", cadir      );
    if(certfile)   dprintf( D_SECURITY, "CERTFILE:   '%s'\n", certfile   );
    if(keyfile)    dprintf( D_SECURITY, "KEYFILE:    '%s'\n", keyfile    );
    if(cipherlist) dprintf( D_SECURITY, "CIPHERLIST: '%s'\n", cipherlist );
        
    ctx = SSL_CTX_new( SSLv23_method(  ) );
	if(!ctx) {
		ouch( "Error creating new SSL context.\n");
		goto setup_server_ctx_err;
	}

	SSL_CTX_set_options( ctx, SSL_OP_NO_SSLv2 );

    if( SSL_CTX_load_verify_locations( ctx, cafile, cadir ) != 1 ) {
        ouch( "Error loading CA file and/or directory\n" );
		goto setup_server_ctx_err;
    }
    if( SSL_CTX_use_certificate_chain_file( ctx, certfile ) != 1 ) {
        ouch( "Error loading certificate from file" );
        goto setup_server_ctx_err;
    }
    priv = set_root_priv();
    if( SSL_CTX_use_PrivateKey_file( ctx, keyfile, SSL_FILETYPE_PEM) != 1 ) {
        set_priv(priv);
        ouch( "Error loading private key from file" );
        goto setup_server_ctx_err;
    }
    set_priv(priv);
    SSL_CTX_set_verify( ctx, SSL_VERIFY_PEER, verify_callback ); 
    SSL_CTX_set_verify_depth( ctx, 4 );  
    SSL_CTX_set_options( ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2 );
    if(SSL_CTX_set_cipher_list( ctx, cipherlist ) != 1 ) {
        ouch( "Error setting cipher list (no valid ciphers)\n" );
        goto setup_server_ctx_err;
    }
    if(cafile)          free(cafile);
    if(cadir)           free(cadir);
    if(certfile)        free(certfile);
    if(keyfile)         free(keyfile);
    if(cipherlist)      free(cipherlist);
    return ctx;
  setup_server_ctx_err:
    if(cafile)          free(cafile);
    if(cadir)           free(cadir);
    if(certfile)        free(certfile);
    if(keyfile)         free(keyfile);
    if(cipherlist)      free(cipherlist);
	if(ctx)		        SSL_CTX_free(ctx);
    return NULL;
}
