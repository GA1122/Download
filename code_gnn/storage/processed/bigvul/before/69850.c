circuit_resume_edge_reading_helper(edge_connection_t *first_conn,
                                   circuit_t *circ,
                                   crypt_path_t *layer_hint)
{
  edge_connection_t *conn;
  int n_packaging_streams, n_streams_left;
  int packaged_this_round;
  int cells_on_queue;
  int cells_per_conn;
  edge_connection_t *chosen_stream = NULL;
  int max_to_package;

  if (first_conn == NULL) {
     
    return 0;
  }

   
  max_to_package = circ->package_window;
  if (CIRCUIT_IS_ORIGIN(circ)) {
    cells_on_queue = circ->n_chan_cells.n;
  } else {
    or_circuit_t *or_circ = TO_OR_CIRCUIT(circ);
    cells_on_queue = or_circ->p_chan_cells.n;
  }
  if (CELL_QUEUE_HIGHWATER_SIZE - cells_on_queue < max_to_package)
    max_to_package = CELL_QUEUE_HIGHWATER_SIZE - cells_on_queue;

   

   
  {
    int num_streams = 0;
    for (conn = first_conn; conn; conn = conn->next_stream) {
      num_streams++;
      if (tor_weak_random_one_in_n(&stream_choice_rng, num_streams)) {
        chosen_stream = conn;
      }
       
    }
  }

   
  n_packaging_streams = 0;
   
  for (conn=chosen_stream; conn; conn = conn->next_stream) {
     
    if (conn->base_.marked_for_close || conn->package_window <= 0)
      continue;
    if (!layer_hint || conn->cpath_layer == layer_hint) {
      connection_start_reading(TO_CONN(conn));

      if (connection_get_inbuf_len(TO_CONN(conn)) > 0)
        ++n_packaging_streams;
    }
  }
   
  for (conn = first_conn; conn != chosen_stream; conn = conn->next_stream) {
    if (conn->base_.marked_for_close || conn->package_window <= 0)
      continue;
    if (!layer_hint || conn->cpath_layer == layer_hint) {
      connection_start_reading(TO_CONN(conn));

      if (connection_get_inbuf_len(TO_CONN(conn)) > 0)
        ++n_packaging_streams;
    }
  }

  if (n_packaging_streams == 0)  
    return 0;

 again:

  cells_per_conn = CEIL_DIV(max_to_package, n_packaging_streams);

  packaged_this_round = 0;
  n_streams_left = 0;

   
  for (conn=first_conn; conn; conn=conn->next_stream) {
    if (conn->base_.marked_for_close || conn->package_window <= 0)
      continue;
    if (!layer_hint || conn->cpath_layer == layer_hint) {
      int n = cells_per_conn, r;
       
      r = connection_edge_package_raw_inbuf(conn, 1, &n);

       
      packaged_this_round += (cells_per_conn-n);

      if (r<0) {
         
        connection_mark_for_close(TO_CONN(conn));
        continue;
      }

       
      if (connection_get_inbuf_len(TO_CONN(conn)))
          ++n_streams_left;

       
      if (circuit_consider_stop_edge_reading(circ, layer_hint))
        return -1;
       
    }
  }

   
  if (packaged_this_round && packaged_this_round < max_to_package &&
      n_streams_left) {
    max_to_package -= packaged_this_round;
    n_packaging_streams = n_streams_left;
    goto again;
  }

  return 0;
}
