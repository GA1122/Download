int Condor_Auth_SSL::authenticate(const char *  , CondorError*  )
{
    long err;
    char *buffer;
    char err_buf[500];
    int ssl_status = AUTH_SSL_A_OK;
    int server_status = AUTH_SSL_A_OK;
    int client_status = AUTH_SSL_A_OK;
    int done = 0;
    int success = (0 == 0);
    int fail = (0 == 1);
    int round_ctr = 0;
    BIO *conn_in = NULL, *conn_out = NULL;
    SSL *ssl = NULL;
    SSL_CTX *ctx = NULL;
	unsigned char session_key[AUTH_SSL_SESSION_KEY_LEN];

     
     

	buffer = (char*) malloc( AUTH_SSL_BUF_SIZE );
    
    if( mySock_->isClient() ) {
        if( init_OpenSSL( ) != AUTH_SSL_A_OK ) {
            ouch( "Error initializing OpenSSL for authentication\n" );
            client_status = AUTH_SSL_ERROR;
        }
        if( !(ctx = setup_ssl_ctx( false )) ) {
            ouch( "Error initializing client security context\n" );
            client_status = AUTH_SSL_ERROR;
        }
        if( !(conn_in = BIO_new( BIO_s_mem( ) )) 
            || !(conn_out = BIO_new( BIO_s_mem( ) )) ) {
            ouch( "Error creating buffer for SSL authentication\n" );
            client_status = AUTH_SSL_ERROR;
        }
        if( !(ssl = SSL_new( ctx )) ) {
            ouch( "Error creating SSL context\n" );
            client_status = AUTH_SSL_ERROR;
        }
        server_status = client_share_status( client_status );
        if( server_status != AUTH_SSL_A_OK || client_status != AUTH_SSL_A_OK ) {
            ouch( "SSL Authentication fails, terminating\n" );
			free(buffer);
            return fail;
        }

        SSL_set_bio( ssl, conn_in, conn_out );

        done = 0;
        round_ctr = 0;

        while( !done ) {
            if( client_status != AUTH_SSL_HOLDING ) {
                ouch("Trying to connect.\n");
                ssl_status = SSL_connect( ssl );
                dprintf(D_SECURITY, "Tried to connect: %d\n", ssl_status);
            }
            if( ssl_status < 1 ) {
                client_status = AUTH_SSL_QUITTING;
                done = 1;
                err = SSL_get_error( ssl, ssl_status );
                switch( err ) {
                case SSL_ERROR_ZERO_RETURN:
                    ouch("SSL: connection has been closed.\n");
                    break;
                case SSL_ERROR_WANT_READ:
                    ouch("SSL: trying to continue reading.\n");
                    client_status = AUTH_SSL_RECEIVING;
                    done = 0;
                    break;
                case SSL_ERROR_WANT_WRITE:
                    ouch("SSL: trying to continue writing.\n");
                    client_status = AUTH_SSL_SENDING;
                    done = 0;
                    break;
                case SSL_ERROR_WANT_CONNECT:
                case SSL_ERROR_WANT_ACCEPT:
                    ouch("SSL: error want connect/accept.\n");
                    break;
                case SSL_ERROR_WANT_X509_LOOKUP:
                    ouch("SSL: X509_LOOKUP: callback incomplete.\n" );
                    break;
                case SSL_ERROR_SYSCALL:
                    ouch("SSL: Syscall.\n" );
                    break;
                case SSL_ERROR_SSL:
                    ouch("SSL: library failure.  see error queue?\n");
                    break;
                default:
                    ouch("SSL: unknown error?\n" );
                    break;
                }
            } else {
                client_status = AUTH_SSL_HOLDING;
            }
            round_ctr++;
            dprintf(D_SECURITY,"Round %d.\n", round_ctr);            
            if(round_ctr % 2 == 1) {
                if(AUTH_SSL_ERROR == client_send_message(
                       client_status, buffer, conn_in, conn_out )) {
                   server_status = AUTH_SSL_QUITTING;
                }
            } else {
                server_status = client_receive_message(
                    client_status, buffer, conn_in, conn_out );
            }
            dprintf(D_SECURITY,"Status (c: %d, s: %d)\n", client_status, server_status);
             
            if( server_status == AUTH_SSL_ERROR ) {
                server_status = AUTH_SSL_QUITTING;
            }
            if( server_status == AUTH_SSL_HOLDING
                && client_status == AUTH_SSL_HOLDING ) {
                done = 1;
            }
            if( client_status == AUTH_SSL_QUITTING
                || server_status == AUTH_SSL_QUITTING ) {
                ouch( "SSL Authentication failed\n" );
				free(buffer);
                return fail;
            }
        }
        dprintf(D_SECURITY,"Client trying post connection check.\n");
        if((err = post_connection_check(
                ssl, AUTH_SSL_ROLE_CLIENT )) != X509_V_OK ) {
            ouch( "Error on check of peer certificate\n" );
            snprintf(err_buf, 500, "%s\n",
                     X509_verify_cert_error_string( err ));
            ouch( err_buf );
            client_status = AUTH_SSL_QUITTING;
        } else {
            client_status = AUTH_SSL_A_OK;
        }

        dprintf(D_SECURITY,"Client performs one last exchange of messages.\n");

        if( client_status == AUTH_SSL_QUITTING
            || server_status == AUTH_SSL_QUITTING ) {
            ouch( "SSL Authentication failed\n" );
			free(buffer);
            return fail;
        }

        client_status = server_status = AUTH_SSL_RECEIVING;
        done = 0;
        round_ctr = 0;
        while(!done) {
            dprintf(D_SECURITY,"Reading round %d.\n",++round_ctr);
            if(round_ctr > 256) {
                ouch("Too many rounds exchanging key: quitting.\n");
                done = 1;
                client_status = AUTH_SSL_QUITTING;
                break;
            }
            if( client_status != AUTH_SSL_HOLDING) {
                ssl_status = SSL_read(ssl, 
									  session_key, AUTH_SSL_SESSION_KEY_LEN);
            }
            if(ssl_status < 1) {
                err = SSL_get_error( ssl, ssl_status);
                switch( err ) {
                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                    ouch("SSL: continue read/write.\n");
                    done = 0;
                    client_status = AUTH_SSL_RECEIVING;
                    break;
                default:
                    client_status = AUTH_SSL_QUITTING;
                    done = 1;
                    ouch("SSL: error on write.  Can't proceed.\n");
                    break;
                }
            } else {
                dprintf(D_SECURITY,"SSL read has succeeded.\n");
                client_status = AUTH_SSL_HOLDING;
            }
            if(round_ctr % 2 == 1) {
                server_status = client_receive_message(
                    client_status, buffer, conn_in, conn_out );
            } else {
                if(AUTH_SSL_ERROR == client_send_message(
                       client_status, buffer, conn_in, conn_out )) {
                    server_status = AUTH_SSL_QUITTING;
                }
            }
            dprintf(D_ALWAYS, "Status: c: %d, s: %d\n", client_status, server_status);
            if(server_status == AUTH_SSL_HOLDING
               && client_status == AUTH_SSL_HOLDING) {
                done = 1;
            }
            if(server_status == AUTH_SSL_QUITTING) {
                done = 1;
            }
        }
        if( server_status == AUTH_SSL_QUITTING
            || client_status == AUTH_SSL_QUITTING ) {
            ouch( "SSL Authentication failed at session key exchange.\n" );
			free(buffer);
            return fail;
        }
        setup_crypto( session_key, AUTH_SSL_SESSION_KEY_LEN );
    } else {  
        
        if( init_OpenSSL(  ) != AUTH_SSL_A_OK ) {
            ouch( "Error initializing OpenSSL for authentication\n" );
            server_status = AUTH_SSL_ERROR;
        }
        if( !(ctx = setup_ssl_ctx( true )) ) {
            ouch( "Error initializing server security context\n" );
            server_status = AUTH_SSL_ERROR;
        }
        if( !(conn_in = BIO_new( BIO_s_mem( ) ))
            || !(conn_out = BIO_new( BIO_s_mem( ) )) ) {
            ouch( "Error creating buffer for SSL authentication\n" );
            server_status = AUTH_SSL_ERROR;
        }
        if (!(ssl = SSL_new(ctx))) {
            ouch("Error creating SSL context\n");
            server_status = AUTH_SSL_ERROR;
        }
        client_status = server_share_status( server_status );
        if( client_status != AUTH_SSL_A_OK
            || server_status != AUTH_SSL_A_OK ) {
            ouch( "SSL Authentication fails, terminating\n" );
			free(buffer);
            return fail;
        }
  
        SSL_set_bio(ssl, conn_in, conn_out);

        done = 0;
        round_ctr = 0;
        while( !done ) {
            if( server_status != AUTH_SSL_HOLDING ) {
                ouch("Trying to accept.\n");
                ssl_status = SSL_accept( ssl );
                dprintf(D_SECURITY, "Accept returned %d.\n", ssl_status);
            }
            if( ssl_status < 1 ) {
                server_status = AUTH_SSL_QUITTING;
                done = 1;
                err = SSL_get_error( ssl, ssl_status );
                switch( err ) {
                case SSL_ERROR_ZERO_RETURN:
                    ouch("SSL: connection has been closed.\n");
                    break;
                case SSL_ERROR_WANT_READ:
                    ouch("SSL: trying to continue reading.\n");
                    server_status = AUTH_SSL_RECEIVING;
                    done = 0;
                    break;
                case SSL_ERROR_WANT_WRITE:
                    ouch("SSL: trying to continue writing.\n");
                    server_status = AUTH_SSL_SENDING;
                    done = 0;
                    break;
                case SSL_ERROR_WANT_CONNECT:
                case SSL_ERROR_WANT_ACCEPT:
                    ouch("SSL: error want connect/accept.\n");
                    break;
                case SSL_ERROR_WANT_X509_LOOKUP:
                    ouch("SSL: X509_LOOKUP: callback incomplete.\n" );
                    break;
                case SSL_ERROR_SYSCALL:
                    ouch("SSL: Syscall.\n" );
                    break;
                case SSL_ERROR_SSL:
                    ouch("SSL: library failure.  see error queue?\n");
                    break;
                default:
                    ouch("SSL: unknown error?\n" );
                    break;
                }
            } else {
                server_status = AUTH_SSL_HOLDING;
            }
            round_ctr++;
            dprintf(D_SECURITY,"Round %d.\n", round_ctr);
            if(round_ctr %2 == 1) {
                client_status = server_receive_message(
                    server_status, buffer, conn_in, conn_out );
            } else {
                if(AUTH_SSL_ERROR == server_send_message(
                       server_status, buffer, conn_in, conn_out )) {
                    client_status = AUTH_SSL_QUITTING;
                }
            }
            dprintf(D_SECURITY,"Status (c: %d, s: %d)\n", client_status, server_status);            
             
            if (client_status == AUTH_SSL_ERROR) {
                client_status = AUTH_SSL_QUITTING;
            }
            if( client_status == AUTH_SSL_HOLDING
                && server_status == AUTH_SSL_HOLDING ) {
                done = 1;
            }
            if( client_status == AUTH_SSL_QUITTING
                || server_status == AUTH_SSL_QUITTING ) {
                ouch( "SSL Authentication failed\n" );
				free(buffer);
                return fail;
            }
        }
        ouch("Server trying post connection check.\n");
        if ((err = post_connection_check(ssl, AUTH_SSL_ROLE_SERVER)) != X509_V_OK) {
            ouch( "Error on check of peer certificate\n" );

            char errbuf[500];
            snprintf(errbuf, 500, "%s\n", X509_verify_cert_error_string(err));
            ouch( errbuf );
            ouch( "Error checking SSL object after connection\n" );
            server_status = AUTH_SSL_QUITTING;
        } else {
            server_status = AUTH_SSL_A_OK;
        }

        if( server_status == AUTH_SSL_QUITTING
            || client_status == AUTH_SSL_QUITTING ) {
            ouch( "SSL Authentication failed\n" );
			free(buffer);
            return fail;
        }
        if(!RAND_bytes(session_key, AUTH_SSL_SESSION_KEY_LEN)) {
            ouch("Couldn't generate session key.\n");
            server_status = AUTH_SSL_QUITTING;
        }

        client_status = server_status = AUTH_SSL_RECEIVING;
        done = 0;
        round_ctr = 0;
        while(!done) {
            dprintf(D_SECURITY,"Writing round %d.\n", ++round_ctr);
            if(round_ctr > 256) {
                ouch("Too many rounds exchanging key: quitting.\n");
                done = 1;
                server_status = AUTH_SSL_QUITTING;
                break;
            }
            if( server_status != AUTH_SSL_HOLDING ) {
                ssl_status = SSL_write(ssl, 
									   session_key, AUTH_SSL_SESSION_KEY_LEN);
            }
            if(ssl_status < 1) {
                err = SSL_get_error( ssl, ssl_status);
                switch( err ) {
                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                    ouch("SSL: continue read/write.\n");
                    done = 0;
                    server_status = AUTH_SSL_RECEIVING;
                    break;
                default:
                    server_status = AUTH_SSL_QUITTING;
                    done = 1;
                    ouch("SSL: error on write.  Can't proceed.\n");
                    break;
                }
            } else {
                dprintf(D_SECURITY, "SSL write has succeeded.\n");
                if(client_status == AUTH_SSL_HOLDING) {
                    done = 1;
                }
                server_status = AUTH_SSL_HOLDING;
            }
            if(round_ctr % 2 == 1) {
                if(AUTH_SSL_ERROR == server_send_message(
                       server_status, buffer, conn_in, conn_out )) {
                    client_status = AUTH_SSL_QUITTING;
                }
            } else {
                client_status = server_receive_message(
                    server_status, buffer, conn_in, conn_out );
            }
            dprintf(D_ALWAYS, "Status: c: %d, s: %d\n", client_status, server_status);
            if(server_status == AUTH_SSL_HOLDING
               && client_status == AUTH_SSL_HOLDING) {
                done = 1;
            }
            if(client_status == AUTH_SSL_QUITTING) {
                done = 1;
            }
        }
        if( server_status == AUTH_SSL_QUITTING
            || client_status == AUTH_SSL_QUITTING ) {
            ouch( "SSL Authentication failed at key exchange.\n" );
			free(buffer);
            return fail;
        }
        setup_crypto( session_key, AUTH_SSL_SESSION_KEY_LEN );
    }

    char subjectname[1024];
    X509 *peer = SSL_get_peer_certificate(ssl);
    X509_NAME_oneline(X509_get_subject_name(peer), subjectname, 1024);
    setAuthenticatedName( subjectname );
    setRemoteUser( "ssl" );
	setRemoteDomain( UNMAPPED_DOMAIN );

    dprintf(D_SECURITY,"SSL authentication succeeded to %s\n", subjectname);
	SSL_CTX_free(ctx);
	SSL_free(ssl);
	free(buffer);
    return success;
}
