connection_edge_finished_connecting(edge_connection_t *edge_conn)
{
  connection_t *conn;

  tor_assert(edge_conn);
  tor_assert(edge_conn->base_.type == CONN_TYPE_EXIT);
  conn = TO_CONN(edge_conn);
  tor_assert(conn->state == EXIT_CONN_STATE_CONNECTING);

  log_info(LD_EXIT,"Exit connection to %s:%u (%s) established.",
           escaped_safe_str(conn->address), conn->port,
           safe_str(fmt_and_decorate_addr(&conn->addr)));

  rep_hist_note_exit_stream_opened(conn->port);

  conn->state = EXIT_CONN_STATE_OPEN;
  connection_watch_events(conn, READ_EVENT);  
  if (connection_get_outbuf_len(conn))  
    connection_start_writing(conn);
   
  if (connection_edge_is_rendezvous_stream(edge_conn)) {
    if (connection_edge_send_command(edge_conn,
                                     RELAY_COMMAND_CONNECTED, NULL, 0) < 0)
      return 0;  
  } else {
    uint8_t connected_payload[MAX_CONNECTED_CELL_PAYLOAD_LEN];
    int connected_payload_len =
      connected_cell_format_payload(connected_payload, &conn->addr,
                                    edge_conn->address_ttl);
    if (connected_payload_len < 0)
      return -1;

    if (connection_edge_send_command(edge_conn,
                        RELAY_COMMAND_CONNECTED,
                        (char*)connected_payload, connected_payload_len) < 0)
      return 0;  
  }
  tor_assert(edge_conn->package_window > 0);
   
  return connection_edge_process_inbuf(edge_conn, 1);
}
