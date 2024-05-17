connection_exit_begin_resolve(cell_t *cell, or_circuit_t *circ)
{
  edge_connection_t *dummy_conn;
  relay_header_t rh;

  assert_circuit_ok(TO_CIRCUIT(circ));
  relay_header_unpack(&rh, cell->payload);
  if (rh.length > RELAY_PAYLOAD_SIZE)
    return -1;

   
  dummy_conn = edge_connection_new(CONN_TYPE_EXIT, AF_INET);
  dummy_conn->stream_id = rh.stream_id;
  dummy_conn->base_.address = tor_strndup(
                                       (char*)cell->payload+RELAY_HEADER_SIZE,
                                       rh.length);
  dummy_conn->base_.port = 0;
  dummy_conn->base_.state = EXIT_CONN_STATE_RESOLVEFAILED;
  dummy_conn->base_.purpose = EXIT_PURPOSE_RESOLVE;

  dummy_conn->on_circuit = TO_CIRCUIT(circ);

   
  switch (dns_resolve(dummy_conn)) {
    case -1:  
       
      return 0;
    case 1:  
      if (!dummy_conn->base_.marked_for_close)
        connection_free(TO_CONN(dummy_conn));
      return 0;
    case 0:  
      assert_circuit_ok(TO_CIRCUIT(circ));
      break;
  }
  return 0;
}
