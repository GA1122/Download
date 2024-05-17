MojoResult DataPipeConsumerDispatcher::EndReadData(uint32_t num_bytes_read) {
  base::AutoLock lock(lock_);
  if (!in_two_phase_read_)
    return MOJO_RESULT_FAILED_PRECONDITION;

  if (in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;

  CHECK(shared_ring_buffer_.IsValid());

  MojoResult rv;
  if (num_bytes_read > two_phase_max_bytes_read_ ||
      num_bytes_read % options_.element_num_bytes != 0) {
    rv = MOJO_RESULT_INVALID_ARGUMENT;
  } else {
    rv = MOJO_RESULT_OK;
    read_offset_ =
        (read_offset_ + num_bytes_read) % options_.capacity_num_bytes;

    DCHECK_GE(bytes_available_, num_bytes_read);
    bytes_available_ -= num_bytes_read;

    base::AutoUnlock unlock(lock_);
    NotifyRead(num_bytes_read);
  }

  in_two_phase_read_ = false;
  two_phase_max_bytes_read_ = 0;

  watchers_.NotifyState(GetHandleSignalsStateNoLock());

  return rv;
}
