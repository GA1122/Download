MojoResult DataPipeProducerDispatcher::WriteData(
    const void* elements,
    uint32_t* num_bytes,
    const MojoWriteDataOptions& options) {
  base::AutoLock lock(lock_);
  if (!shared_ring_buffer_.IsValid() || in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;

  if (in_two_phase_write_)
    return MOJO_RESULT_BUSY;

  if (peer_closed_)
    return MOJO_RESULT_FAILED_PRECONDITION;

  if (*num_bytes % options_.element_num_bytes != 0)
    return MOJO_RESULT_INVALID_ARGUMENT;
  if (*num_bytes == 0)
    return MOJO_RESULT_OK;   

  if ((options.flags & MOJO_WRITE_DATA_FLAG_ALL_OR_NONE) &&
      (*num_bytes > available_capacity_)) {
    return MOJO_RESULT_OUT_OF_RANGE;
  }

  DCHECK_LE(available_capacity_, options_.capacity_num_bytes);
  uint32_t num_bytes_to_write = std::min(*num_bytes, available_capacity_);
  if (num_bytes_to_write == 0)
    return MOJO_RESULT_SHOULD_WAIT;

  *num_bytes = num_bytes_to_write;

  CHECK(ring_buffer_mapping_.IsValid());
  uint8_t* data = static_cast<uint8_t*>(ring_buffer_mapping_.memory());
  CHECK(data);

  const uint8_t* source = static_cast<const uint8_t*>(elements);
  CHECK(source);

  DCHECK_LE(write_offset_, options_.capacity_num_bytes);
  uint32_t tail_bytes_to_write =
      std::min(options_.capacity_num_bytes - write_offset_, num_bytes_to_write);
  uint32_t head_bytes_to_write = num_bytes_to_write - tail_bytes_to_write;

  DCHECK_GT(tail_bytes_to_write, 0u);
  memcpy(data + write_offset_, source, tail_bytes_to_write);
  if (head_bytes_to_write > 0)
    memcpy(data, source + tail_bytes_to_write, head_bytes_to_write);

  DCHECK_LE(num_bytes_to_write, available_capacity_);
  available_capacity_ -= num_bytes_to_write;
  write_offset_ =
      (write_offset_ + num_bytes_to_write) % options_.capacity_num_bytes;

  watchers_.NotifyState(GetHandleSignalsStateNoLock());

  base::AutoUnlock unlock(lock_);
  NotifyWrite(num_bytes_to_write);

  return MOJO_RESULT_OK;
}
