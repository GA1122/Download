int Condor_Auth_SSL :: server_exchange_messages( int server_status, char *buf, BIO *conn_in, BIO *conn_out )
{
    ouch("Server exchange messages.\n");
    if(server_send_message( server_status, buf, conn_in, conn_out )
       == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
    return server_receive_message( server_status, buf, conn_in, conn_out );
}
