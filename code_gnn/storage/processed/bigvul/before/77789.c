static CURLcode create_conn(struct Curl_easy *data,
                            struct connectdata **in_connect,
                            bool *async)
{
  CURLcode result = CURLE_OK;
  struct connectdata *conn;
  struct connectdata *conn_temp = NULL;
  bool reuse;
  bool connections_available = TRUE;
  bool force_reuse = FALSE;
  bool waitpipe = FALSE;
  size_t max_host_connections = Curl_multi_max_host_connections(data->multi);
  size_t max_total_connections = Curl_multi_max_total_connections(data->multi);

  *async = FALSE;

   
  if(!data->change.url) {
    result = CURLE_URL_MALFORMAT;
    goto out;
  }

   
  conn = allocate_conn(data);

  if(!conn) {
    result = CURLE_OUT_OF_MEMORY;
    goto out;
  }

   
  *in_connect = conn;

  result = parseurlandfillconn(data, conn);
  if(result)
    goto out;

  if(data->set.str[STRING_BEARER]) {
    conn->oauth_bearer = strdup(data->set.str[STRING_BEARER]);
    if(!conn->oauth_bearer) {
      result = CURLE_OUT_OF_MEMORY;
      goto out;
    }
  }

#ifdef USE_UNIX_SOCKETS
  if(data->set.str[STRING_UNIX_SOCKET_PATH]) {
    conn->unix_domain_socket = strdup(data->set.str[STRING_UNIX_SOCKET_PATH]);
    if(conn->unix_domain_socket == NULL) {
      result = CURLE_OUT_OF_MEMORY;
      goto out;
    }
    conn->abstract_unix_socket = data->set.abstract_unix_socket;
  }
#endif

   
#ifndef CURL_DISABLE_PROXY
  result = create_conn_helper_init_proxy(conn);
  if(result)
    goto out;
#endif

   
  if((conn->given->flags&PROTOPT_SSL) && conn->bits.httpproxy)
    conn->bits.tunnel_proxy = TRUE;

   
  result = parse_remote_port(data, conn);
  if(result)
    goto out;

   
  result = override_login(data, conn, &conn->user, &conn->passwd,
                          &conn->options);
  if(result)
    goto out;

  result = set_login(conn);  
  if(result)
    goto out;

   
  result = parse_connect_to_slist(data, conn, data->set.connect_to);
  if(result)
    goto out;

   
  result = fix_hostname(conn, &conn->host);
  if(result)
    goto out;
  if(conn->bits.conn_to_host) {
    result = fix_hostname(conn, &conn->conn_to_host);
    if(result)
      goto out;
  }
  if(conn->bits.httpproxy) {
    result = fix_hostname(conn, &conn->http_proxy.host);
    if(result)
      goto out;
  }
  if(conn->bits.socksproxy) {
    result = fix_hostname(conn, &conn->socks_proxy.host);
    if(result)
      goto out;
  }

   
  if(conn->bits.conn_to_host &&
     strcasecompare(conn->conn_to_host.name, conn->host.name)) {
    conn->bits.conn_to_host = FALSE;
  }

   
  if(conn->bits.conn_to_port && conn->conn_to_port == conn->remote_port) {
    conn->bits.conn_to_port = FALSE;
  }

   
  if((conn->bits.conn_to_host || conn->bits.conn_to_port) &&
      conn->bits.httpproxy)
    conn->bits.tunnel_proxy = TRUE;

   
  result = setup_connection_internals(conn);
  if(result)
    goto out;

  conn->recv[FIRSTSOCKET] = Curl_recv_plain;
  conn->send[FIRSTSOCKET] = Curl_send_plain;
  conn->recv[SECONDARYSOCKET] = Curl_recv_plain;
  conn->send[SECONDARYSOCKET] = Curl_send_plain;

  conn->bits.tcp_fastopen = data->set.tcp_fastopen;

   
#ifndef CURL_DISABLE_FILE
  if(conn->handler->flags & PROTOPT_NONETWORK) {
    bool done;
     
    DEBUGASSERT(conn->handler->connect_it);
    Curl_persistconninfo(conn);
    result = conn->handler->connect_it(conn, &done);

     
    if(!result) {
      conn->data = data;
      conn->bits.tcpconnect[FIRSTSOCKET] = TRUE;  

      result = Curl_conncache_add_conn(data->state.conn_cache, conn);
      if(result)
        goto out;

       
      result = setup_range(data);
      if(result) {
        DEBUGASSERT(conn->handler->done);
         
        (void)conn->handler->done(conn, result, FALSE);
        goto out;
      }

      Curl_setup_transfer(conn, -1, -1, FALSE, NULL,  
                          -1, NULL);  
    }

     
    Curl_init_do(data, conn);

    goto out;
  }
#endif

   
  data->set.ssl.primary.CApath = data->set.str[STRING_SSL_CAPATH_ORIG];
  data->set.proxy_ssl.primary.CApath = data->set.str[STRING_SSL_CAPATH_PROXY];
  data->set.ssl.primary.CAfile = data->set.str[STRING_SSL_CAFILE_ORIG];
  data->set.proxy_ssl.primary.CAfile = data->set.str[STRING_SSL_CAFILE_PROXY];
  data->set.ssl.primary.random_file = data->set.str[STRING_SSL_RANDOM_FILE];
  data->set.proxy_ssl.primary.random_file =
    data->set.str[STRING_SSL_RANDOM_FILE];
  data->set.ssl.primary.egdsocket = data->set.str[STRING_SSL_EGDSOCKET];
  data->set.proxy_ssl.primary.egdsocket = data->set.str[STRING_SSL_EGDSOCKET];
  data->set.ssl.primary.cipher_list =
    data->set.str[STRING_SSL_CIPHER_LIST_ORIG];
  data->set.proxy_ssl.primary.cipher_list =
    data->set.str[STRING_SSL_CIPHER_LIST_PROXY];
  data->set.ssl.primary.cipher_list13 =
    data->set.str[STRING_SSL_CIPHER13_LIST_ORIG];
  data->set.proxy_ssl.primary.cipher_list13 =
    data->set.str[STRING_SSL_CIPHER13_LIST_PROXY];

  data->set.ssl.CRLfile = data->set.str[STRING_SSL_CRLFILE_ORIG];
  data->set.proxy_ssl.CRLfile = data->set.str[STRING_SSL_CRLFILE_PROXY];
  data->set.ssl.issuercert = data->set.str[STRING_SSL_ISSUERCERT_ORIG];
  data->set.proxy_ssl.issuercert = data->set.str[STRING_SSL_ISSUERCERT_PROXY];
  data->set.ssl.cert = data->set.str[STRING_CERT_ORIG];
  data->set.proxy_ssl.cert = data->set.str[STRING_CERT_PROXY];
  data->set.ssl.cert_type = data->set.str[STRING_CERT_TYPE_ORIG];
  data->set.proxy_ssl.cert_type = data->set.str[STRING_CERT_TYPE_PROXY];
  data->set.ssl.key = data->set.str[STRING_KEY_ORIG];
  data->set.proxy_ssl.key = data->set.str[STRING_KEY_PROXY];
  data->set.ssl.key_type = data->set.str[STRING_KEY_TYPE_ORIG];
  data->set.proxy_ssl.key_type = data->set.str[STRING_KEY_TYPE_PROXY];
  data->set.ssl.key_passwd = data->set.str[STRING_KEY_PASSWD_ORIG];
  data->set.proxy_ssl.key_passwd = data->set.str[STRING_KEY_PASSWD_PROXY];
  data->set.ssl.primary.clientcert = data->set.str[STRING_CERT_ORIG];
  data->set.proxy_ssl.primary.clientcert = data->set.str[STRING_CERT_PROXY];
#ifdef USE_TLS_SRP
  data->set.ssl.username = data->set.str[STRING_TLSAUTH_USERNAME_ORIG];
  data->set.proxy_ssl.username = data->set.str[STRING_TLSAUTH_USERNAME_PROXY];
  data->set.ssl.password = data->set.str[STRING_TLSAUTH_PASSWORD_ORIG];
  data->set.proxy_ssl.password = data->set.str[STRING_TLSAUTH_PASSWORD_PROXY];
#endif

  if(!Curl_clone_primary_ssl_config(&data->set.ssl.primary,
     &conn->ssl_config)) {
    result = CURLE_OUT_OF_MEMORY;
    goto out;
  }

  if(!Curl_clone_primary_ssl_config(&data->set.proxy_ssl.primary,
                                    &conn->proxy_ssl_config)) {
    result = CURLE_OUT_OF_MEMORY;
    goto out;
  }

  prune_dead_connections(data);

   

  DEBUGASSERT(conn->user);
  DEBUGASSERT(conn->passwd);

   
  if(data->set.reuse_fresh && !data->state.this_is_a_follow)
    reuse = FALSE;
  else
    reuse = ConnectionExists(data, conn, &conn_temp, &force_reuse, &waitpipe);

   
  if(reuse && !force_reuse && IsPipeliningPossible(data, conn_temp)) {
    size_t pipelen = conn_temp->send_pipe.size + conn_temp->recv_pipe.size;
    if(pipelen > 0) {
      infof(data, "Found connection %ld, with requests in the pipe (%zu)\n",
            conn_temp->connection_id, pipelen);

      if(Curl_conncache_bundle_size(conn_temp) < max_host_connections &&
         Curl_conncache_size(data) < max_total_connections) {
         
        reuse = FALSE;

        infof(data, "We can reuse, but we want a new connection anyway\n");
        Curl_conncache_return_conn(conn_temp);
      }
    }
  }

  if(reuse) {
     
    reuse_conn(conn, conn_temp);
#ifdef USE_SSL
    free(conn->ssl_extra);
#endif
    free(conn);           
    conn = conn_temp;
    *in_connect = conn;

    infof(data, "Re-using existing connection! (#%ld) with %s %s\n",
          conn->connection_id,
          conn->bits.proxy?"proxy":"host",
          conn->socks_proxy.host.name ? conn->socks_proxy.host.dispname :
          conn->http_proxy.host.name ? conn->http_proxy.host.dispname :
                                       conn->host.dispname);
  }
  else {
     

    if(conn->handler->flags & PROTOPT_ALPN_NPN) {
       
      if(data->set.ssl_enable_alpn)
        conn->bits.tls_enable_alpn = TRUE;
      if(data->set.ssl_enable_npn)
        conn->bits.tls_enable_npn = TRUE;
    }

    if(waitpipe)
       
      connections_available = FALSE;
    else {
       
      struct connectbundle *bundle =
        Curl_conncache_find_bundle(conn, data->state.conn_cache);

      if(max_host_connections > 0 && bundle &&
         (bundle->num_connections >= max_host_connections)) {
        struct connectdata *conn_candidate;

         
        conn_candidate = Curl_conncache_extract_bundle(data, bundle);
        Curl_conncache_unlock(conn);

        if(conn_candidate)
          (void)Curl_disconnect(data, conn_candidate,
                                  FALSE);
        else {
          infof(data, "No more connections allowed to host: %zu\n",
                max_host_connections);
          connections_available = FALSE;
        }
      }
      else
        Curl_conncache_unlock(conn);

    }

    if(connections_available &&
       (max_total_connections > 0) &&
       (Curl_conncache_size(data) >= max_total_connections)) {
      struct connectdata *conn_candidate;

       
      conn_candidate = Curl_conncache_extract_oldest(data);
      if(conn_candidate)
        (void)Curl_disconnect(data, conn_candidate,
                                FALSE);
      else {
        infof(data, "No connections available in cache\n");
        connections_available = FALSE;
      }
    }

    if(!connections_available) {
      infof(data, "No connections available.\n");

      conn_free(conn);
      *in_connect = NULL;

      result = CURLE_NO_CONNECTION_AVAILABLE;
      goto out;
    }
    else {
       
      result = Curl_conncache_add_conn(data->state.conn_cache, conn);
      if(result)
        goto out;
    }

#if defined(USE_NTLM)
     
    if((data->state.authhost.picked & (CURLAUTH_NTLM | CURLAUTH_NTLM_WB)) &&
       data->state.authhost.done) {
      infof(data, "NTLM picked AND auth done set, clear picked!\n");
      data->state.authhost.picked = CURLAUTH_NONE;
      data->state.authhost.done = FALSE;
    }

    if((data->state.authproxy.picked & (CURLAUTH_NTLM | CURLAUTH_NTLM_WB)) &&
       data->state.authproxy.done) {
      infof(data, "NTLM-proxy picked AND auth done set, clear picked!\n");
      data->state.authproxy.picked = CURLAUTH_NONE;
      data->state.authproxy.done = FALSE;
    }
#endif
  }

   
  Curl_init_do(data, conn);

   
  result = setup_range(data);
  if(result)
    goto out;

   

   
  conn->seek_func = data->set.seek_func;
  conn->seek_client = data->set.seek_client;

   
  result = resolve_server(data, conn, async);

out:
  return result;
}
