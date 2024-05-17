connection_ap_handshake_rewrite_and_attach(entry_connection_t *conn,
                                           origin_circuit_t *circ,
                                           crypt_path_t *cpath)
{
  socks_request_t *socks = conn->socks_request;
  const or_options_t *options = get_options();
  connection_t *base_conn = ENTRY_TO_CONN(conn);
  time_t now = time(NULL);
  rewrite_result_t rr;

   
  memset(&rr, 0, sizeof(rr));
  connection_ap_handshake_rewrite(conn,&rr);

  if (rr.should_close) {
     
    connection_mark_unattached_ap(conn, rr.end_reason);
    if (END_STREAM_REASON_DONE == (rr.end_reason & END_STREAM_REASON_MASK))
      return 0;
    else
      return -1;
  }

  const time_t map_expires = rr.map_expires;
  const int automap = rr.automap;
  const addressmap_entry_source_t exit_source = rr.exit_source;

   
  const hostname_type_t addresstype = parse_extended_hostname(socks->address);

   
  if (addresstype == BAD_HOSTNAME) {
    control_event_client_status(LOG_WARN, "SOCKS_BAD_HOSTNAME HOSTNAME=%s",
                                escaped(socks->address));
    connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
    return -1;
  }

   
  if (addresstype == EXIT_HOSTNAME) {
     
    routerset_t *excludeset = options->StrictNodes ?
      options->ExcludeExitNodesUnion_ : options->ExcludeExitNodes;
    const node_t *node = NULL;

     
    if (exit_source == ADDRMAPSRC_AUTOMAP && !options->AllowDotExit) {
       
      log_warn(LD_APP,"Stale automapped address for '%s.exit', with "
               "AllowDotExit disabled. Refusing.",
               safe_str_client(socks->address));
      control_event_client_status(LOG_WARN, "SOCKS_BAD_HOSTNAME HOSTNAME=%s",
                                  escaped(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
      return -1;
    }

     
    if (exit_source == ADDRMAPSRC_DNS ||
        (exit_source == ADDRMAPSRC_NONE && !options->AllowDotExit)) {
       
      log_warn(LD_BUG,"Address '%s.exit', with impossible source for the "
               ".exit part. Refusing.",
               safe_str_client(socks->address));
      control_event_client_status(LOG_WARN, "SOCKS_BAD_HOSTNAME HOSTNAME=%s",
                                  escaped(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
      return -1;
    }

    tor_assert(!automap);

     
    char *s = strrchr(socks->address,'.');
    if (s) {
       
      if (s[1] != '\0') {
         
        conn->chosen_exit_name = tor_strdup(s+1);
        node = node_get_by_nickname(conn->chosen_exit_name, 1);

        if (exit_source == ADDRMAPSRC_TRACKEXIT) {
           
          conn->chosen_exit_retries = TRACKHOSTEXITS_RETRIES;
        }
        *s = 0;
      } else {
         
        log_warn(LD_APP,"Malformed exit address '%s.exit'. Refusing.",
                 safe_str_client(socks->address));
        control_event_client_status(LOG_WARN, "SOCKS_BAD_HOSTNAME HOSTNAME=%s",
                                    escaped(socks->address));
        connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
        return -1;
      }
    } else {
       

      conn->chosen_exit_name = tor_strdup(socks->address);
      node = node_get_by_nickname(conn->chosen_exit_name, 1);
      if (node) {
        *socks->address = 0;
        node_get_address_string(node, socks->address, sizeof(socks->address));
      }
    }

     
    if (!node) {
      log_warn(LD_APP,
               "Unrecognized relay in exit address '%s.exit'. Refusing.",
               safe_str_client(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
      return -1;
    }
     
    if (routerset_contains_node(excludeset, node)) {
      log_warn(LD_APP,
               "Excluded relay in exit address '%s.exit'. Refusing.",
               safe_str_client(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
      return -1;
    }
     
  }

   
  if (addresstype != ONION_HOSTNAME) {
     

     
    if (address_is_invalid_destination(socks->address, 1)) {
      control_event_client_status(LOG_WARN, "SOCKS_BAD_HOSTNAME HOSTNAME=%s",
                                  escaped(socks->address));
      log_warn(LD_APP,
               "Destination '%s' seems to be an invalid hostname. Failing.",
               safe_str_client(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
      return -1;
    }

#ifdef ENABLE_TOR2WEB_MODE
     
    if (options->Tor2webMode) {
      log_warn(LD_APP, "Refusing to connect to non-hidden-service hostname "
               "or IP address %s because tor2web mode is enabled.",
               safe_str_client(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
      return -1;
    }
#endif

     

     
    if (!conn->entry_cfg.dns_request && !conn->entry_cfg.ipv4_traffic
        && !conn->entry_cfg.ipv6_traffic) {
        log_warn(LD_APP, "Refusing to connect to non-hidden-service hostname "
                 "or IP address %s because Port has OnionTrafficOnly set (or "
                 "NoDNSRequest, NoIPv4Traffic, and NoIPv6Traffic).",
                 safe_str_client(socks->address));
        connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
        return -1;
    }

     
    tor_addr_t dummy_addr;
    int socks_family = tor_addr_parse(&dummy_addr, socks->address);
     
    if (socks_family == -1) {
      if (!conn->entry_cfg.dns_request) {
        log_warn(LD_APP, "Refusing to connect to hostname %s "
                 "because Port has NoDNSRequest set.",
                 safe_str_client(socks->address));
        connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
        return -1;
      }
    } else if (socks_family == AF_INET) {
      if (!conn->entry_cfg.ipv4_traffic) {
        log_warn(LD_APP, "Refusing to connect to IPv4 address %s because "
                 "Port has NoIPv4Traffic set.",
                 safe_str_client(socks->address));
        connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
        return -1;
      }
    } else if (socks_family == AF_INET6) {
      if (!conn->entry_cfg.ipv6_traffic) {
        log_warn(LD_APP, "Refusing to connect to IPv6 address %s because "
                 "Port has NoIPv6Traffic set.",
                 safe_str_client(socks->address));
        connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
        return -1;
      }
    } else {
      tor_assert_nonfatal_unreached_once();
    }

     
    if (socks->command == SOCKS_COMMAND_RESOLVE) {
      tor_addr_t answer;
       
      if (tor_addr_parse(&answer, socks->address) >= 0) { 
         
        strlcpy(socks->address, rr.orig_address, sizeof(socks->address));
        connection_ap_handshake_socks_resolved_addr(conn, &answer, -1,
                                                    map_expires);
        connection_mark_unattached_ap(conn,
                                END_STREAM_REASON_DONE |
                                END_STREAM_REASON_FLAG_ALREADY_SOCKS_REPLIED);
        return 0;
      }
      tor_assert(!automap);
      rep_hist_note_used_resolve(now);  
    } else if (socks->command == SOCKS_COMMAND_CONNECT) {
       

      tor_assert(!automap);
       
      if (socks->port == 0) {
        log_notice(LD_APP,"Application asked to connect to port 0. Refusing.");
        connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
        return -1;
      }
       
      if (options->ClientRejectInternalAddresses &&
          !conn->use_begindir && !conn->chosen_exit_name && !circ) {
         
        tor_addr_t addr;
        if (tor_addr_hostname_is_local(socks->address) ||
            (tor_addr_parse(&addr, socks->address) >= 0 &&
             tor_addr_is_internal(&addr, 0))) {
           
          if (conn->is_transparent_ap) {
#define WARN_INTRVL_LOOP 300
            static ratelim_t loop_warn_limit = RATELIM_INIT(WARN_INTRVL_LOOP);
            char *m;
            if ((m = rate_limit_log(&loop_warn_limit, approx_time()))) {
              log_warn(LD_NET,
                       "Rejecting request for anonymous connection to private "
                       "address %s on a TransPort or NATDPort.  Possible loop "
                       "in your NAT rules?%s", safe_str_client(socks->address),
                       m);
              tor_free(m);
            }
          } else {
#define WARN_INTRVL_PRIV 300
            static ratelim_t priv_warn_limit = RATELIM_INIT(WARN_INTRVL_PRIV);
            char *m;
            if ((m = rate_limit_log(&priv_warn_limit, approx_time()))) {
              log_warn(LD_NET,
                       "Rejecting SOCKS request for anonymous connection to "
                       "private address %s.%s",
                       safe_str_client(socks->address),m);
              tor_free(m);
            }
          }
          connection_mark_unattached_ap(conn, END_STREAM_REASON_PRIVATE_ADDR);
          return -1;
        }
      }  

       
      {
        tor_addr_t addr;
         
        if (tor_addr_parse(&addr, socks->address) >= 0) {
           
          sa_family_t family = tor_addr_family(&addr);

          if ((family == AF_INET && ! conn->entry_cfg.ipv4_traffic) ||
              (family == AF_INET6 && ! conn->entry_cfg.ipv6_traffic)) {
             
            log_warn(LD_NET, "Rejecting SOCKS request for an IP address "
                     "family that this listener does not support.");
            connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
            return -1;
          } else if (family == AF_INET6 && socks->socks_version == 4) {
             
            log_warn(LD_NET, "Rejecting SOCKS4 request for an IPv6 address.");
            connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
            return -1;
          } else if (socks->socks_version == 4 &&
                     !conn->entry_cfg.ipv4_traffic) {
             
            log_warn(LD_NET, "Rejecting SOCKS4 request on a listener with "
                     "no IPv4 traffic supported.");
            connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
            return -1;
          } else if (family == AF_INET6) {
             
            conn->entry_cfg.ipv4_traffic = 0;
          } else if (family == AF_INET) {
             
            conn->entry_cfg.ipv6_traffic = 0;
          }
        }
      }

       
      if (socks->socks_version == 4)
        conn->entry_cfg.ipv6_traffic = 0;

       
      if (!conn->use_begindir && !conn->chosen_exit_name && !circ) {
         
        const node_t *r =
          router_find_exact_exit_enclave(socks->address, socks->port);
        if (r) {
          log_info(LD_APP,
                   "Redirecting address %s to exit at enclave router %s",
                   safe_str_client(socks->address), node_describe(r));
           
          conn->chosen_exit_name =
            tor_strdup(hex_str(r->identity, DIGEST_LEN));
          conn->chosen_exit_optional = 1;
        }
      }

       
      if (!conn->use_begindir && !conn->chosen_exit_name && !circ)
        if (consider_plaintext_ports(conn, socks->port) < 0)
          return -1;

       
      if (!conn->use_begindir) {
         
        rep_hist_note_used_port(now, socks->port);
      }
    } else if (socks->command == SOCKS_COMMAND_RESOLVE_PTR) {
      rep_hist_note_used_resolve(now);  
       
    } else {
       
      tor_fragile_assert();
    }

     
    base_conn->state = AP_CONN_STATE_CIRCUIT_WAIT;

     
    int rv;
    if (circ) {
      rv = connection_ap_handshake_attach_chosen_circuit(conn, circ, cpath);
    } else {
       
      connection_ap_mark_as_pending_circuit(conn);
      rv = 0;
    }

     
    if (rv < 0) {
      if (!base_conn->marked_for_close)
        connection_mark_unattached_ap(conn, END_STREAM_REASON_CANT_ATTACH);
      return -1;
    }

    return 0;
  } else {
     
    tor_assert(!automap);

     
    if (!conn->entry_cfg.onion_traffic) {
      log_warn(LD_APP, "Onion address %s requested from a port with .onion "
                       "disabled", safe_str_client(socks->address));
      connection_mark_unattached_ap(conn, END_STREAM_REASON_ENTRYPOLICY);
      return -1;
    }

     
    if (SOCKS_COMMAND_IS_RESOLVE(socks->command)) {
       
      log_warn(LD_APP,
               "Resolve requests to hidden services not allowed. Failing.");
      connection_ap_handshake_socks_resolved(conn,RESOLVED_TYPE_ERROR,
                                             0,NULL,-1,TIME_MAX);
      connection_mark_unattached_ap(conn,
                                END_STREAM_REASON_SOCKSPROTOCOL |
                                END_STREAM_REASON_FLAG_ALREADY_SOCKS_REPLIED);
      return -1;
    }

     
    if (circ) {
      log_warn(LD_CONTROL, "Attachstream to a circuit is not "
               "supported for .onion addresses currently. Failing.");
      connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
      return -1;
    }

     
    rend_service_authorization_t *client_auth =
      rend_client_lookup_service_authorization(socks->address);

    const uint8_t *cookie = NULL;
    rend_auth_type_t auth_type = REND_NO_AUTH;
    if (client_auth) {
      log_info(LD_REND, "Using previously configured client authorization "
               "for hidden service request.");
      auth_type = client_auth->auth_type;
      cookie = client_auth->descriptor_cookie;
    }

     
    rend_data_t *rend_data = ENTRY_TO_EDGE_CONN(conn)->rend_data =
      rend_data_client_create(socks->address, NULL, (char *) cookie,
                              auth_type);
    if (rend_data == NULL) {
      return -1;
    }
    const char *onion_address = rend_data_get_address(rend_data);
    log_info(LD_REND,"Got a hidden service request for ID '%s'",
             safe_str_client(onion_address));

     
    unsigned int refetch_desc = 0;
    rend_cache_entry_t *entry = NULL;
    const int rend_cache_lookup_result =
      rend_cache_lookup_entry(onion_address, -1, &entry);
    if (rend_cache_lookup_result < 0) {
      switch (-rend_cache_lookup_result) {
      case EINVAL:
         
        log_warn(LD_BUG,"Invalid service name '%s'",
                 safe_str_client(onion_address));
        connection_mark_unattached_ap(conn, END_STREAM_REASON_TORPROTOCOL);
        return -1;
      case ENOENT:
         
        refetch_desc = 1;
        break;
      default:
        log_warn(LD_BUG, "Unknown cache lookup error %d",
            rend_cache_lookup_result);
        return -1;
      }
    }

     
    rep_hist_note_used_internal(now, 0, 1);

     
    if (refetch_desc || !rend_client_any_intro_points_usable(entry)) {
      connection_ap_mark_as_non_pending_circuit(conn);
      base_conn->state = AP_CONN_STATE_RENDDESC_WAIT;
      log_info(LD_REND, "Unknown descriptor %s. Fetching.",
               safe_str_client(onion_address));
      rend_client_refetch_v2_renddesc(rend_data);
      return 0;
    }

     
    base_conn->state = AP_CONN_STATE_CIRCUIT_WAIT;
    log_info(LD_REND, "Descriptor is here. Great.");

     
    connection_ap_mark_as_pending_circuit(conn);
    return 0;
  }

  return 0;  
}
