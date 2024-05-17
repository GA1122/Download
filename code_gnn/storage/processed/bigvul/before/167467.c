MojoResult DataPipeConsumerDispatcher::BeginReadData(
    const void** buffer,
    uint32_t* buffer_num_bytes) {
  base::AutoLock lock(lock_);
  if (!shared_ring_buffer_.IsValid() || in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;

  if (in_two_phase_read_)
    return MOJO_RESULT_BUSY;

  const bool had_new_data = new_data_available_;
  new_data_available_ = false;

  if (bytes_available_ == 0) {
    if (had_new_data)
      watchers_.NotifyState(GetHandleSignalsStateNoLock());
    return peer_closed_ ? MOJO_RESULT_FAILED_PRECONDITION
                        : MOJO_RESULT_SHOULD_WAIT;
  }

  DCHECK_LT(read_offset_, options_.capacity_num_bytes);
  uint32_t bytes_to_read =
      std::min(bytes_available_, options_.capacity_num_bytes - read_offset_);

  CHECK(ring_buffer_mapping_.IsValid());
  uint8_t* data = static_cast<uint8_t*>(ring_buffer_mapping_.memory());
  CHECK(data);

  in_two_phase_read_ = true;
  *buffer = data + read_offset_;
  *buffer_num_bytes = bytes_to_read;
  two_phase_max_bytes_read_ = bytes_to_read;

  if (had_new_data)
    watchers_.NotifyState(GetHandleSignalsStateNoLock());

  return MOJO_RESULT_OK;
}
