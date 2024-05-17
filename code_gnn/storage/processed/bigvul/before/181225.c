 accept_xsmp_connection (SmsConn        sms_conn,
                         GsmXsmpServer *server,
                         unsigned long *mask_ret,
                          SmsCallbacks  *callbacks_ret,
                          char         **failure_reason_ret)
  {
        IceConn        ice_conn;
        GsmXSMPClient *client;
//         IceConn                ice_conn;
//         GsmClient             *client;
//         GsmIceConnectionWatch *data;
  
           
          if (server->priv->xsmp_sockets == NULL) {
                 g_debug ("GsmXsmpServer: In shutdown, rejecting new client");
 
                 *failure_reason_ret = strdup (_("Refusing new client connection because the session is currently being shut down\n"));
                 return FALSE;
          }
  
          ice_conn = SmsGetIceConnection (sms_conn);
        client = ice_conn->context;
//         data = ice_conn->context;
  
        g_return_val_if_fail (client != NULL, TRUE);
//          
//         free_ice_connection_watch (data);
// 
//         client = gsm_xsmp_client_new (ice_conn);
// 
//         gsm_store_add (server->priv->client_store, gsm_client_peek_id (client), G_OBJECT (client));
//          
//         g_object_unref (client);
  
        gsm_xsmp_client_connect (client, sms_conn, mask_ret, callbacks_ret);
//         gsm_xsmp_client_connect (GSM_XSMP_CLIENT (client), sms_conn, mask_ret, callbacks_ret);
  
          return TRUE;
  }