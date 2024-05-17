MojoResult DataPipeProducerDispatcher::BeginWriteData(
    void** buffer,
    uint32_t* buffer_num_bytes) {
  base::AutoLock lock(lock_);
  if (!shared_ring_buffer_.IsValid() || in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;

  if (in_two_phase_write_)
    return MOJO_RESULT_BUSY;
  if (peer_closed_)
    return MOJO_RESULT_FAILED_PRECONDITION;

  if (available_capacity_ == 0) {
    return peer_closed_ ? MOJO_RESULT_FAILED_PRECONDITION
                        : MOJO_RESULT_SHOULD_WAIT;
  }

  in_two_phase_write_ = true;
  *buffer_num_bytes = std::min(options_.capacity_num_bytes - write_offset_,
                               available_capacity_);
  DCHECK_GT(*buffer_num_bytes, 0u);

  CHECK(ring_buffer_mapping_.IsValid());
  uint8_t* data = static_cast<uint8_t*>(ring_buffer_mapping_.memory());
  *buffer = data + write_offset_;

  return MOJO_RESULT_OK;
}
