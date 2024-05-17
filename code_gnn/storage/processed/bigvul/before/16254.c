int Condor_Auth_SSL :: receive_message( int &status, int &len, char *buf )
{
    ouch("Receive message.\n");
    mySock_ ->decode( );
    if( !(mySock_ ->code( status ))
        || !(mySock_ ->code( len ))
        || !(len == (mySock_ ->get_bytes( buf, len )))
        || !(mySock_ ->end_of_message( )) ) {
        ouch( "Error communicating with peer.\n" );
        return AUTH_SSL_ERROR;
    }
    dprintf(D_SECURITY,"Received message (%d).\n", status );
    return AUTH_SSL_A_OK;
}
