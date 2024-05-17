channel_flush_from_first_active_circuit(channel_t *chan, int max)
{
  circuitmux_t *cmux = NULL;
  int n_flushed = 0;
  cell_queue_t *queue;
  circuit_t *circ;
  or_circuit_t *or_circ;
  int streams_blocked;
  packed_cell_t *cell;

   
  tor_assert(chan);
  tor_assert(chan->cmux);
  cmux = chan->cmux;

   
  while (n_flushed < max) {
    circ = circuitmux_get_first_active_circuit(cmux);
     
    if (!circ) break;
    assert_cmux_ok_paranoid(chan);

    if (circ->n_chan == chan) {
      queue = &circ->n_chan_cells;
      streams_blocked = circ->streams_blocked_on_n_chan;
    } else {
      or_circ = TO_OR_CIRCUIT(circ);
      tor_assert(or_circ->p_chan == chan);
      queue = &TO_OR_CIRCUIT(circ)->p_chan_cells;
      streams_blocked = circ->streams_blocked_on_p_chan;
    }

     
    tor_assert(queue->n > 0);

     
    cell = cell_queue_pop(queue);

     
    if (get_options()->CellStatistics && !CIRCUIT_IS_ORIGIN(circ)) {
      uint32_t msec_waiting;
      struct timeval tvnow;
      or_circ = TO_OR_CIRCUIT(circ);
      tor_gettimeofday_cached(&tvnow);
      msec_waiting = ((uint32_t)tv_to_msec(&tvnow)) - cell->inserted_time;

      or_circ->total_cell_waiting_time += msec_waiting;
      or_circ->processed_cells++;
    }

     
    if (queue->n == 0 && chan->dirreq_id)
      geoip_change_dirreq_state(chan->dirreq_id,
                                DIRREQ_TUNNELED,
                                DIRREQ_CIRC_QUEUE_FLUSHED);

     
    channel_write_packed_cell(chan, cell);
    cell = NULL;

     

     
    ++n_flushed;

     
    circuitmux_notify_xmit_cells(cmux, circ, 1);
    circuitmux_set_num_cells(cmux, circ, queue->n);
    if (queue->n == 0)
      log_debug(LD_GENERAL, "Made a circuit inactive.");

     
    if (streams_blocked && queue->n <= CELL_QUEUE_LOWWATER_SIZE)
      set_streams_blocked_on_circ(circ, chan, 0, 0);  

     
  }

   
  assert_cmux_ok_paranoid(chan);

  return n_flushed;
}
