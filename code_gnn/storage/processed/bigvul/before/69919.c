connection_edge_reached_eof(edge_connection_t *conn)
{
  if (connection_get_inbuf_len(TO_CONN(conn)) &&
      connection_state_is_open(TO_CONN(conn))) {
     
    return 0;
  }
  log_info(LD_EDGE,"conn (fd "TOR_SOCKET_T_FORMAT") reached eof. Closing.",
           conn->base_.s);
  if (!conn->base_.marked_for_close) {
     
    connection_edge_end(conn, END_STREAM_REASON_DONE);
    if (conn->base_.type == CONN_TYPE_AP) {
       
      if (EDGE_TO_ENTRY_CONN(conn)->socks_request)
        EDGE_TO_ENTRY_CONN(conn)->socks_request->has_finished = 1;
    }
    connection_mark_for_close(TO_CONN(conn));
  }
  return 0;
}
