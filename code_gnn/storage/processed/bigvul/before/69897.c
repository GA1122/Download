connection_ap_handshake_socks_reply(entry_connection_t *conn, char *reply,
                                    size_t replylen, int endreason)
{
  char buf[256];
  socks5_reply_status_t status =
    stream_end_reason_to_socks5_response(endreason);

  tor_assert(conn->socks_request);  

  if (!SOCKS_COMMAND_IS_RESOLVE(conn->socks_request->command)) {
    control_event_stream_status(conn, status==SOCKS5_SUCCEEDED ?
                                STREAM_EVENT_SUCCEEDED : STREAM_EVENT_FAILED,
                                endreason);
  }

   
  if (status == SOCKS5_SUCCEEDED ||
      endreason == END_STREAM_REASON_RESOLVEFAILED ||
      endreason == END_STREAM_REASON_CONNECTREFUSED ||
      endreason == END_STREAM_REASON_CONNRESET ||
      endreason == END_STREAM_REASON_NOROUTE ||
      endreason == END_STREAM_REASON_RESOURCELIMIT) {
    if (!conn->edge_.on_circuit ||
       !CIRCUIT_IS_ORIGIN(conn->edge_.on_circuit)) {
      if (endreason != END_STREAM_REASON_RESOLVEFAILED) {
        log_info(LD_BUG,
                 "No origin circuit for successful SOCKS stream "U64_FORMAT
                 ". Reason: %d",
                 U64_PRINTF_ARG(ENTRY_TO_CONN(conn)->global_identifier),
                 endreason);
      }
       
    } else {
      pathbias_mark_use_success(TO_ORIGIN_CIRCUIT(conn->edge_.on_circuit));
    }
  }

  if (conn->socks_request->has_finished) {
    log_warn(LD_BUG, "(Harmless.) duplicate calls to "
             "connection_ap_handshake_socks_reply.");
    return;
  }
  if (replylen) {  
    connection_write_to_buf(reply, replylen, ENTRY_TO_CONN(conn));
    conn->socks_request->has_finished = 1;
    return;
  }
  if (conn->socks_request->socks_version == 4) {
    memset(buf,0,SOCKS4_NETWORK_LEN);
    buf[1] = (status==SOCKS5_SUCCEEDED ? SOCKS4_GRANTED : SOCKS4_REJECT);
     
    connection_write_to_buf(buf, SOCKS4_NETWORK_LEN, ENTRY_TO_CONN(conn));
  } else if (conn->socks_request->socks_version == 5) {
    size_t buf_len;
    memset(buf,0,sizeof(buf));
    if (tor_addr_family(&conn->edge_.base_.addr) == AF_INET) {
      buf[0] = 5;  
      buf[1] = (char)status;
      buf[2] = 0;
      buf[3] = 1;  
       
      buf_len = 10;
    } else {  
      buf[0] = 5;  
      buf[1] = (char)status;
      buf[2] = 0;
      buf[3] = 4;  
       
      buf_len = 22;
    }
    connection_write_to_buf(buf,buf_len,ENTRY_TO_CONN(conn));
  }
   
  conn->socks_request->has_finished = 1;
  return;
}
