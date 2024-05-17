MojoResult DataPipeProducerDispatcher::EndWriteData(
    uint32_t num_bytes_written) {
  base::AutoLock lock(lock_);
  if (is_closed_ || in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;

  if (!in_two_phase_write_)
    return MOJO_RESULT_FAILED_PRECONDITION;

  MojoResult rv = MOJO_RESULT_OK;
  if (num_bytes_written > available_capacity_ ||
      num_bytes_written % options_.element_num_bytes != 0 ||
      write_offset_ + num_bytes_written > options_.capacity_num_bytes) {
    rv = MOJO_RESULT_INVALID_ARGUMENT;
  } else {
    DCHECK_LE(num_bytes_written + write_offset_, options_.capacity_num_bytes);
    available_capacity_ -= num_bytes_written;
    write_offset_ =
        (write_offset_ + num_bytes_written) % options_.capacity_num_bytes;

    base::AutoUnlock unlock(lock_);
    NotifyWrite(num_bytes_written);
  }

  in_two_phase_write_ = false;

  watchers_.NotifyState(GetHandleSignalsStateNoLock());

  return rv;
}
