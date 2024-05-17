MOCK_IMPL(void,
connection_ap_handshake_socks_resolved,(entry_connection_t *conn,
                                       int answer_type,
                                       size_t answer_len,
                                       const uint8_t *answer,
                                       int ttl,
                                       time_t expires))
{
  char buf[384];
  size_t replylen;

  if (ttl >= 0) {
    if (answer_type == RESOLVED_TYPE_IPV4 && answer_len == 4) {
      tor_addr_t a;
      tor_addr_from_ipv4n(&a, get_uint32(answer));
      if (! tor_addr_is_null(&a)) {
        client_dns_set_addressmap(conn,
                                  conn->socks_request->address, &a,
                                  conn->chosen_exit_name, ttl);
      }
    } else if (answer_type == RESOLVED_TYPE_IPV6 && answer_len == 16) {
      tor_addr_t a;
      tor_addr_from_ipv6_bytes(&a, (char*)answer);
      if (! tor_addr_is_null(&a)) {
        client_dns_set_addressmap(conn,
                                  conn->socks_request->address, &a,
                                  conn->chosen_exit_name, ttl);
      }
    } else if (answer_type == RESOLVED_TYPE_HOSTNAME && answer_len < 256) {
      char *cp = tor_strndup((char*)answer, answer_len);
      client_dns_set_reverse_addressmap(conn,
                                        conn->socks_request->address,
                                        cp,
                                        conn->chosen_exit_name, ttl);
      tor_free(cp);
    }
  }

  if (ENTRY_TO_EDGE_CONN(conn)->is_dns_request) {
    if (conn->dns_server_request) {
       
      dnsserv_resolved(conn, answer_type, answer_len, (char*)answer, ttl);
      conn->socks_request->has_finished = 1;
      return;
    } else {
       
      tell_controller_about_resolved_result(conn, answer_type, answer_len,
                                            (char*)answer, ttl, expires);
      conn->socks_request->has_finished = 1;
      return;
    }
     
  }

  if (conn->socks_request->socks_version == 4) {
    buf[0] = 0x00;  
    if (answer_type == RESOLVED_TYPE_IPV4 && answer_len == 4) {
      buf[1] = SOCKS4_GRANTED;
      set_uint16(buf+2, 0);
      memcpy(buf+4, answer, 4);  
      replylen = SOCKS4_NETWORK_LEN;
    } else {  
      buf[1] = SOCKS4_REJECT;
      memset(buf+2, 0, 6);
      replylen = SOCKS4_NETWORK_LEN;
    }
  } else if (conn->socks_request->socks_version == 5) {
     
    buf[0] = 0x05;  
    if (answer_type == RESOLVED_TYPE_IPV4 && answer_len == 4) {
      buf[1] = SOCKS5_SUCCEEDED;
      buf[2] = 0;  
      buf[3] = 0x01;  
      memcpy(buf+4, answer, 4);  
      set_uint16(buf+8, 0);  
      replylen = 10;
    } else if (answer_type == RESOLVED_TYPE_IPV6 && answer_len == 16) {
      buf[1] = SOCKS5_SUCCEEDED;
      buf[2] = 0;  
      buf[3] = 0x04;  
      memcpy(buf+4, answer, 16);  
      set_uint16(buf+20, 0);  
      replylen = 22;
    } else if (answer_type == RESOLVED_TYPE_HOSTNAME && answer_len < 256) {
      buf[1] = SOCKS5_SUCCEEDED;
      buf[2] = 0;  
      buf[3] = 0x03;  
      buf[4] = (char)answer_len;
      memcpy(buf+5, answer, answer_len);  
      set_uint16(buf+5+answer_len, 0);  
      replylen = 5+answer_len+2;
    } else {
      buf[1] = SOCKS5_HOST_UNREACHABLE;
      memset(buf+2, 0, 8);
      replylen = 10;
    }
  } else {
     
    return;
  }
  connection_ap_handshake_socks_reply(conn, buf, replylen,
          (answer_type == RESOLVED_TYPE_IPV4 ||
           answer_type == RESOLVED_TYPE_IPV6 ||
           answer_type == RESOLVED_TYPE_HOSTNAME) ?
                                      0 : END_STREAM_REASON_RESOLVEFAILED);
}
