ConnectionExists(struct Curl_easy *data,
                 struct connectdata *needle,
                 struct connectdata **usethis,
                 bool *force_reuse,
                 bool *waitpipe)
{
  struct connectdata *check;
  struct connectdata *chosen = 0;
  bool foundPendingCandidate = FALSE;
  int canpipe = IsPipeliningPossible(data, needle);
  struct connectbundle *bundle;

#ifdef USE_NTLM
  bool wantNTLMhttp = ((data->state.authhost.want &
                      (CURLAUTH_NTLM | CURLAUTH_NTLM_WB)) &&
                      (needle->handler->protocol & PROTO_FAMILY_HTTP));
  bool wantProxyNTLMhttp = (needle->bits.proxy_user_passwd &&
                           ((data->state.authproxy.want &
                           (CURLAUTH_NTLM | CURLAUTH_NTLM_WB)) &&
                           (needle->handler->protocol & PROTO_FAMILY_HTTP)));
#endif

  *force_reuse = FALSE;
  *waitpipe = FALSE;

   
  if((canpipe & CURLPIPE_HTTP1) &&
     Curl_pipeline_site_blacklisted(data, needle))
    canpipe &= ~ CURLPIPE_HTTP1;

   
  bundle = Curl_conncache_find_bundle(needle, data->state.conn_cache);
  if(bundle) {
     
    size_t max_pipe_len = (bundle->multiuse != BUNDLE_MULTIPLEX)?
      max_pipeline_length(data->multi):0;
    size_t best_pipe_len = max_pipe_len;
    struct curl_llist_element *curr;

    infof(data, "Found bundle for host %s: %p [%s]\n",
          (needle->bits.conn_to_host ? needle->conn_to_host.name :
           needle->host.name), (void *)bundle,
          (bundle->multiuse == BUNDLE_PIPELINING ?
           "can pipeline" :
           (bundle->multiuse == BUNDLE_MULTIPLEX ?
            "can multiplex" : "serially")));

     
    if(canpipe) {
      if(bundle->multiuse <= BUNDLE_UNKNOWN) {
        if((bundle->multiuse == BUNDLE_UNKNOWN) && data->set.pipewait) {
          infof(data, "Server doesn't support multi-use yet, wait\n");
          *waitpipe = TRUE;
          Curl_conncache_unlock(needle);
          return FALSE;  
        }

        infof(data, "Server doesn't support multi-use (yet)\n");
        canpipe = 0;
      }
      if((bundle->multiuse == BUNDLE_PIPELINING) &&
         !Curl_pipeline_wanted(data->multi, CURLPIPE_HTTP1)) {
         
        infof(data, "Could pipeline, but not asked to!\n");
        canpipe = 0;
      }
      else if((bundle->multiuse == BUNDLE_MULTIPLEX) &&
              !Curl_pipeline_wanted(data->multi, CURLPIPE_MULTIPLEX)) {
        infof(data, "Could multiplex, but not asked to!\n");
        canpipe = 0;
      }
    }

    curr = bundle->conn_list.head;
    while(curr) {
      bool match = FALSE;
      size_t pipeLen;

       
      check = curr->ptr;
      curr = curr->next;

      if(extract_if_dead(check, data)) {
         
        (void)Curl_disconnect(data, check,  TRUE);
        continue;
      }

      pipeLen = check->send_pipe.size + check->recv_pipe.size;

      if(canpipe) {
        if(check->bits.protoconnstart && check->bits.close)
          continue;

        if(!check->bits.multiplex) {
           
          struct Curl_easy* sh = gethandleathead(&check->send_pipe);
          struct Curl_easy* rh = gethandleathead(&check->recv_pipe);
          if(sh) {
            if(!(IsPipeliningPossible(sh, check) & CURLPIPE_HTTP1))
              continue;
          }
          else if(rh) {
            if(!(IsPipeliningPossible(rh, check) & CURLPIPE_HTTP1))
              continue;
          }
        }
      }
      else {
        if(pipeLen > 0) {
           
          continue;
        }

        if(Curl_resolver_asynch()) {
           
          if(!check->ip_addr_str[0]) {
            infof(data,
                  "Connection #%ld is still name resolving, can't reuse\n",
                  check->connection_id);
            continue;
          }
        }

        if((check->sock[FIRSTSOCKET] == CURL_SOCKET_BAD) ||
           check->bits.close) {
          if(!check->bits.close)
            foundPendingCandidate = TRUE;
           
          infof(data, "Connection #%ld isn't open enough, can't reuse\n",
                check->connection_id);
#ifdef DEBUGBUILD
          if(check->recv_pipe.size > 0) {
            infof(data,
                  "BAD! Unconnected #%ld has a non-empty recv pipeline!\n",
                  check->connection_id);
          }
#endif
          continue;
        }
      }

#ifdef USE_UNIX_SOCKETS
      if(needle->unix_domain_socket) {
        if(!check->unix_domain_socket)
          continue;
        if(strcmp(needle->unix_domain_socket, check->unix_domain_socket))
          continue;
        if(needle->abstract_unix_socket != check->abstract_unix_socket)
          continue;
      }
      else if(check->unix_domain_socket)
        continue;
#endif

      if((needle->handler->flags&PROTOPT_SSL) !=
         (check->handler->flags&PROTOPT_SSL))
         
        if(get_protocol_family(check->handler->protocol) !=
           needle->handler->protocol || !check->tls_upgraded)
           
          continue;

      if(needle->bits.httpproxy != check->bits.httpproxy ||
         needle->bits.socksproxy != check->bits.socksproxy)
        continue;

      if(needle->bits.socksproxy && !proxy_info_matches(&needle->socks_proxy,
                                                        &check->socks_proxy))
        continue;

      if(needle->bits.conn_to_host != check->bits.conn_to_host)
         
        continue;

      if(needle->bits.conn_to_port != check->bits.conn_to_port)
         
        continue;

      if(needle->bits.httpproxy) {
        if(!proxy_info_matches(&needle->http_proxy, &check->http_proxy))
          continue;

        if(needle->bits.tunnel_proxy != check->bits.tunnel_proxy)
          continue;

        if(needle->http_proxy.proxytype == CURLPROXY_HTTPS) {
           
          if(needle->handler->flags&PROTOPT_SSL) {
             
            if(!Curl_ssl_config_matches(&needle->proxy_ssl_config,
                                        &check->proxy_ssl_config))
              continue;
            if(check->proxy_ssl[FIRSTSOCKET].state != ssl_connection_complete)
              continue;
          }
          else {
            if(!Curl_ssl_config_matches(&needle->ssl_config,
                                        &check->ssl_config))
              continue;
            if(check->ssl[FIRSTSOCKET].state != ssl_connection_complete)
              continue;
          }
        }
      }

      if(!canpipe && CONN_INUSE(check))
         
        continue;

      if(CONN_INUSE(check) && (check->data->multi != needle->data->multi))
         
        continue;

      if(needle->localdev || needle->localport) {
         
        if((check->localport != needle->localport) ||
           (check->localportrange != needle->localportrange) ||
           (needle->localdev &&
            (!check->localdev || strcmp(check->localdev, needle->localdev))))
          continue;
      }

      if(!(needle->handler->flags & PROTOPT_CREDSPERREQUEST)) {
         
        if(strcmp(needle->user, check->user) ||
           strcmp(needle->passwd, check->passwd)) {
           
          continue;
        }
      }

      if(!needle->bits.httpproxy || (needle->handler->flags&PROTOPT_SSL) ||
         needle->bits.tunnel_proxy) {
         

        if((strcasecompare(needle->handler->scheme, check->handler->scheme) ||
            (get_protocol_family(check->handler->protocol) ==
             needle->handler->protocol && check->tls_upgraded)) &&
           (!needle->bits.conn_to_host || strcasecompare(
            needle->conn_to_host.name, check->conn_to_host.name)) &&
           (!needle->bits.conn_to_port ||
             needle->conn_to_port == check->conn_to_port) &&
           strcasecompare(needle->host.name, check->host.name) &&
           needle->remote_port == check->remote_port) {
           
          if(needle->handler->flags & PROTOPT_SSL) {
             
            if(!Curl_ssl_config_matches(&needle->ssl_config,
                                        &check->ssl_config)) {
              DEBUGF(infof(data,
                           "Connection #%ld has different SSL parameters, "
                           "can't reuse\n",
                           check->connection_id));
              continue;
            }
            if(check->ssl[FIRSTSOCKET].state != ssl_connection_complete) {
              foundPendingCandidate = TRUE;
              DEBUGF(infof(data,
                           "Connection #%ld has not started SSL connect, "
                           "can't reuse\n",
                           check->connection_id));
              continue;
            }
          }
          match = TRUE;
        }
      }
      else {
         
        match = TRUE;
      }

      if(match) {
#if defined(USE_NTLM)
         
        if(wantNTLMhttp) {
          if(strcmp(needle->user, check->user) ||
             strcmp(needle->passwd, check->passwd))
            continue;
        }
        else if(check->ntlm.state != NTLMSTATE_NONE) {
           
          continue;
        }

         
        if(wantProxyNTLMhttp) {
           
          if(!check->http_proxy.user || !check->http_proxy.passwd)
            continue;

          if(strcmp(needle->http_proxy.user, check->http_proxy.user) ||
             strcmp(needle->http_proxy.passwd, check->http_proxy.passwd))
            continue;
        }
        else if(check->proxyntlm.state != NTLMSTATE_NONE) {
           
          continue;
        }

        if(wantNTLMhttp || wantProxyNTLMhttp) {
           
          chosen = check;

          if((wantNTLMhttp &&
             (check->ntlm.state != NTLMSTATE_NONE)) ||
              (wantProxyNTLMhttp &&
               (check->proxyntlm.state != NTLMSTATE_NONE))) {
             
            *force_reuse = TRUE;
            break;
          }

           
          continue;
        }
#endif
        if(canpipe) {
           

          if(pipeLen == 0) {
             
            chosen = check;
            break;
          }

           
          if(max_pipe_len && (pipeLen >= max_pipe_len)) {
            infof(data, "Pipe is full, skip (%zu)\n", pipeLen);
            continue;
          }
#ifdef USE_NGHTTP2
           
          if(check->bits.multiplex) {
             
            struct http_conn *httpc = &check->proto.httpc;
            if(pipeLen >= httpc->settings.max_concurrent_streams) {
              infof(data, "MAX_CONCURRENT_STREAMS reached, skip (%zu)\n",
                    pipeLen);
              continue;
            }
          }
#endif
           
          if(Curl_pipeline_penalized(data, check)) {
            infof(data, "Penalized, skip\n");
            continue;
          }

          if(max_pipe_len) {
            if(pipeLen < best_pipe_len) {
               
              chosen = check;
              best_pipe_len = pipeLen;
              continue;
            }
          }
          else {
             
            chosen = check;
            infof(data, "Multiplexed connection found!\n");
            break;
          }
        }
        else {
           
          chosen = check;
          break;
        }
      }
    }
  }

  if(chosen) {
     
    chosen->data = data;  
    Curl_conncache_unlock(needle);
    *usethis = chosen;
    return TRUE;  
  }
  Curl_conncache_unlock(needle);

  if(foundPendingCandidate && data->set.pipewait) {
    infof(data,
          "Found pending candidate for reuse and CURLOPT_PIPEWAIT is set\n");
    *waitpipe = TRUE;
  }

  return FALSE;  
}
