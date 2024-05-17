connection_ap_handshake_rewrite(entry_connection_t *conn,
                                rewrite_result_t *out)
{
  socks_request_t *socks = conn->socks_request;
  const or_options_t *options = get_options();
  tor_addr_t addr_tmp;

   
  out->automap = 0;
  out->exit_source = ADDRMAPSRC_NONE;
  out->map_expires = TIME_MAX;
  out->end_reason = 0;
  out->should_close = 0;
  out->orig_address[0] = 0;

   
  tor_strlower(socks->address);
   
  strlcpy(out->orig_address, socks->address, sizeof(out->orig_address));
  log_debug(LD_APP,"Client asked for %s:%d",
            safe_str_client(socks->address),
            socks->port);

   
  if (!strcmpend(socks->address, ".exit") && !options->AllowDotExit) {
    log_warn(LD_APP, "The  \".exit\" notation is disabled in Tor due to "
             "security risks. Set AllowDotExit in your torrc to enable "
             "it (at your own risk).");
    control_event_client_status(LOG_WARN, "SOCKS_BAD_HOSTNAME HOSTNAME=%s",
                                escaped(socks->address));
    out->end_reason = END_STREAM_REASON_TORPROTOCOL;
    out->should_close = 1;
    return;
  }

   
   
  if (! conn->original_dest_address) {
     
    conn->original_dest_address = tor_strdup(conn->socks_request->address);
  }

   
  if (socks->command != SOCKS_COMMAND_RESOLVE_PTR) {
    const unsigned rewrite_flags = AMR_FLAG_USE_MAPADDRESS;
    if (addressmap_rewrite(socks->address, sizeof(socks->address),
                       rewrite_flags, &out->map_expires, &out->exit_source)) {
      control_event_stream_status(conn, STREAM_EVENT_REMAP,
                                  REMAP_STREAM_SOURCE_CACHE);
    }
  }

   
  if (socks->command == SOCKS_COMMAND_RESOLVE &&
      tor_addr_parse(&addr_tmp, socks->address)<0 &&
      options->AutomapHostsOnResolve) {
     
    out->automap = addressmap_address_should_automap(socks->address, options);
    if (out->automap) {
       
      const char *new_addr;
       
      int addr_type = RESOLVED_TYPE_IPV4;
      if (conn->socks_request->socks_version != 4) {
        if (!conn->entry_cfg.ipv4_traffic ||
            (conn->entry_cfg.ipv6_traffic && conn->entry_cfg.prefer_ipv6) ||
            conn->entry_cfg.prefer_ipv6_virtaddr)
          addr_type = RESOLVED_TYPE_IPV6;
      }
       
      new_addr = addressmap_register_virtual_address(
                                    addr_type, tor_strdup(socks->address));
      if (! new_addr) {
        log_warn(LD_APP, "Unable to automap address %s",
                 escaped_safe_str(socks->address));
        out->end_reason = END_STREAM_REASON_INTERNAL;
        out->should_close = 1;
        return;
      }
      log_info(LD_APP, "Automapping %s to %s",
               escaped_safe_str_client(socks->address),
               safe_str_client(new_addr));
      strlcpy(socks->address, new_addr, sizeof(socks->address));
    }
  }

   
  if (socks->command == SOCKS_COMMAND_RESOLVE_PTR) {
    unsigned rewrite_flags = 0;
    if (conn->entry_cfg.use_cached_ipv4_answers)
      rewrite_flags |= AMR_FLAG_USE_IPV4_DNS;
    if (conn->entry_cfg.use_cached_ipv6_answers)
      rewrite_flags |= AMR_FLAG_USE_IPV6_DNS;

    if (addressmap_rewrite_reverse(socks->address, sizeof(socks->address),
                                   rewrite_flags, &out->map_expires)) {
      char *result = tor_strdup(socks->address);
       
      tor_snprintf(socks->address, sizeof(socks->address), "REVERSE[%s]",
                   out->orig_address);
      connection_ap_handshake_socks_resolved(conn, RESOLVED_TYPE_HOSTNAME,
                                             strlen(result), (uint8_t*)result,
                                             -1,
                                             out->map_expires);
      tor_free(result);
      out->end_reason = END_STREAM_REASON_DONE |
                        END_STREAM_REASON_FLAG_ALREADY_SOCKS_REPLIED;
      out->should_close = 1;
      return;
    }

     
    if (options->ClientDNSRejectInternalAddresses) {
       
      tor_addr_t addr;
      int ok;
      ok = tor_addr_parse_PTR_name(
                               &addr, socks->address, AF_UNSPEC, 1);
      if (ok == 1 && tor_addr_is_internal(&addr, 0)) {
        connection_ap_handshake_socks_resolved(conn, RESOLVED_TYPE_ERROR,
                                               0, NULL, -1, TIME_MAX);
        out->end_reason = END_STREAM_REASON_SOCKSPROTOCOL |
                          END_STREAM_REASON_FLAG_ALREADY_SOCKS_REPLIED;
        out->should_close = 1;
        return;
      }
    }
  }

   
  if (socks->command != SOCKS_COMMAND_RESOLVE_PTR &&
      !out->automap) {
    unsigned rewrite_flags = AMR_FLAG_USE_AUTOMAP | AMR_FLAG_USE_TRACKEXIT;
    addressmap_entry_source_t exit_source2;
    if (conn->entry_cfg.use_cached_ipv4_answers)
      rewrite_flags |= AMR_FLAG_USE_IPV4_DNS;
    if (conn->entry_cfg.use_cached_ipv6_answers)
      rewrite_flags |= AMR_FLAG_USE_IPV6_DNS;
    if (addressmap_rewrite(socks->address, sizeof(socks->address),
                        rewrite_flags, &out->map_expires, &exit_source2)) {
      control_event_stream_status(conn, STREAM_EVENT_REMAP,
                                  REMAP_STREAM_SOURCE_CACHE);
    }
    if (out->exit_source == ADDRMAPSRC_NONE) {
       
      out->exit_source = exit_source2;
    }
  }

   
  if (!out->automap && address_is_in_virtual_range(socks->address)) {
     
    log_warn(LD_APP,"Missing mapping for virtual address '%s'. Refusing.",
             safe_str_client(socks->address));
    out->end_reason = END_STREAM_REASON_INTERNAL;
    out->should_close = 1;
    return;
  }
}
