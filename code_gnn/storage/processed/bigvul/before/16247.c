int Condor_Auth_SSL :: client_share_status( int client_status )
{
    int server_status;
    if( receive_status( server_status ) == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
    if( send_status( client_status ) == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
    return server_status;
}
