HandleSignalsState DataPipeProducerDispatcher::GetHandleSignalsStateNoLock()
    const {
  lock_.AssertAcquired();
  HandleSignalsState rv;
  if (!peer_closed_) {
    if (!in_two_phase_write_ && shared_ring_buffer_.IsValid() &&
        available_capacity_ > 0)
      rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_WRITABLE;
    if (peer_remote_)
      rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_PEER_REMOTE;
    rv.satisfiable_signals |=
        MOJO_HANDLE_SIGNAL_WRITABLE | MOJO_HANDLE_SIGNAL_PEER_REMOTE;
  } else {
    rv.satisfied_signals |= MOJO_HANDLE_SIGNAL_PEER_CLOSED;
  }
  rv.satisfiable_signals |= MOJO_HANDLE_SIGNAL_PEER_CLOSED;
  return rv;
}
