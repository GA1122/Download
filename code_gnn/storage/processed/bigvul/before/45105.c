static int lua_websocket_close(lua_State *L) 
{
    apr_socket_t *sock;
    char prelude[2];
    request_rec *r = ap_lua_check_request_rec(L, 1);
    
    sock = ap_get_conn_socket(r->connection);
    
     
    prelude[0] = 0x88;  
    prelude[1] = 0;  
    ap_rwrite(prelude, 2, r);
    
     
    apr_socket_close(sock);
    r->output_filters = NULL;
    r->connection->keepalive = AP_CONN_CLOSE;
    return 0;
}
