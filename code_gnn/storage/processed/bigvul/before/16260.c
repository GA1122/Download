int Condor_Auth_SSL :: server_send_message( int server_status, char *buf, BIO *  , BIO *conn_out )
{
    int len;
    buf[0] = 0;  
    len = BIO_read( conn_out, buf, AUTH_SSL_BUF_SIZE );
    if (len < 0) {
        len = 0;
    }
    if( send_message( server_status, buf, len ) == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
    return AUTH_SSL_A_OK;
}
