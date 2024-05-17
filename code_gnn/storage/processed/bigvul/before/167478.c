HandleSignalsState DataPipeConsumerDispatcher::GetHandleSignalsStateNoLock()
    const {
  lock_.AssertAcquired();

  HandleSignalsState rv;
  if (shared_ring_buffer_.IsValid() && bytes_available_) {
    if (!in_two_phase_read_) {
      rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_READABLE;
      if (new_data_available_)
        rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_NEW_DATA_READABLE;
    }
    rv.satisfiable_signals |= MOJO_HANDLE_SIGNAL_READABLE;
  } else if (!peer_closed_ && shared_ring_buffer_.IsValid()) {
    rv.satisfiable_signals |= MOJO_HANDLE_SIGNAL_READABLE;
  }

  if (shared_ring_buffer_.IsValid()) {
    if (new_data_available_ || !peer_closed_)
      rv.satisfiable_signals |= MOJO_HANDLE_SIGNAL_NEW_DATA_READABLE;
  }

  if (peer_closed_) {
    rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_PEER_CLOSED;
  } else {
    rv.satisfiable_signals |= MOJO_HANDLE_SIGNAL_PEER_REMOTE;
    if (peer_remote_)
      rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_PEER_REMOTE;
  }
  rv.satisfiable_signals |= MOJO_HANDLE_SIGNAL_PEER_CLOSED;

  return rv;
}
