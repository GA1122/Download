int Condor_Auth_SSL :: server_receive_message( int  , char *buf, BIO *conn_in, BIO *  )
{
    int client_status;
    int len;
    int rv;
    int written;
    if( receive_message( client_status, len, buf ) == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
        if( len > 0 ) {
            written = 0;
            while( written < len ) {
                rv =  BIO_write( conn_in, buf, len );
                if( rv <= 0 ) {
                    ouch( "Couldn't write connection data into bio\n" );
                    return AUTH_SSL_ERROR;
                    break;
                }
                written += rv;
            }
        }
    return client_status;
}
