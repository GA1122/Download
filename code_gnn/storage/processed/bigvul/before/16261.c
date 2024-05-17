int Condor_Auth_SSL :: server_share_status( int server_status )
{
    int client_status;
    if( send_status( server_status ) == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
    if( receive_status( client_status ) == AUTH_SSL_ERROR ) {
        return AUTH_SSL_ERROR;
    }
    return client_status;
}
