MojoResult DataPipeConsumerDispatcher::ReadData(
    const MojoReadDataOptions& options,
    void* elements,
    uint32_t* num_bytes) {
  base::AutoLock lock(lock_);

  if (!shared_ring_buffer_.IsValid() || in_transit_)
    return MOJO_RESULT_INVALID_ARGUMENT;

  if (in_two_phase_read_)
    return MOJO_RESULT_BUSY;

  const bool had_new_data = new_data_available_;
  new_data_available_ = false;

  if ((options.flags & MOJO_READ_DATA_FLAG_QUERY)) {
    if ((options.flags & MOJO_READ_DATA_FLAG_PEEK) ||
        (options.flags & MOJO_READ_DATA_FLAG_DISCARD))
      return MOJO_RESULT_INVALID_ARGUMENT;
    DCHECK(!(options.flags & MOJO_READ_DATA_FLAG_DISCARD));   
    DVLOG_IF(2, elements) << "Query mode: ignoring non-null |elements|";
    *num_bytes = static_cast<uint32_t>(bytes_available_);

    if (had_new_data)
      watchers_.NotifyState(GetHandleSignalsStateNoLock());
    return MOJO_RESULT_OK;
  }

  bool discard = false;
  if ((options.flags & MOJO_READ_DATA_FLAG_DISCARD)) {
    if (options.flags & MOJO_READ_DATA_FLAG_PEEK)
      return MOJO_RESULT_INVALID_ARGUMENT;
    DVLOG_IF(2, elements) << "Discard mode: ignoring non-null |elements|";
    discard = true;
  }

  uint32_t max_num_bytes_to_read = *num_bytes;
  if (max_num_bytes_to_read % options_.element_num_bytes != 0)
    return MOJO_RESULT_INVALID_ARGUMENT;

  bool all_or_none = options.flags & MOJO_READ_DATA_FLAG_ALL_OR_NONE;
  uint32_t min_num_bytes_to_read = all_or_none ? max_num_bytes_to_read : 0;

  if (min_num_bytes_to_read > bytes_available_) {
    if (had_new_data)
      watchers_.NotifyState(GetHandleSignalsStateNoLock());
    return peer_closed_ ? MOJO_RESULT_FAILED_PRECONDITION
                        : MOJO_RESULT_OUT_OF_RANGE;
  }

  uint32_t bytes_to_read = std::min(max_num_bytes_to_read, bytes_available_);
  if (bytes_to_read == 0) {
    if (had_new_data)
      watchers_.NotifyState(GetHandleSignalsStateNoLock());
    return peer_closed_ ? MOJO_RESULT_FAILED_PRECONDITION
                        : MOJO_RESULT_SHOULD_WAIT;
  }

  if (!discard) {
    const uint8_t* data =
        static_cast<const uint8_t*>(ring_buffer_mapping_.memory());
    CHECK(data);

    uint8_t* destination = static_cast<uint8_t*>(elements);
    CHECK(destination);

    DCHECK_LE(read_offset_, options_.capacity_num_bytes);
    uint32_t tail_bytes_to_copy =
        std::min(options_.capacity_num_bytes - read_offset_, bytes_to_read);
    uint32_t head_bytes_to_copy = bytes_to_read - tail_bytes_to_copy;
    if (tail_bytes_to_copy > 0)
      memcpy(destination, data + read_offset_, tail_bytes_to_copy);
    if (head_bytes_to_copy > 0)
      memcpy(destination + tail_bytes_to_copy, data, head_bytes_to_copy);
  }
  *num_bytes = bytes_to_read;

  bool peek = !!(options.flags & MOJO_READ_DATA_FLAG_PEEK);
  if (discard || !peek) {
    read_offset_ = (read_offset_ + bytes_to_read) % options_.capacity_num_bytes;
    bytes_available_ -= bytes_to_read;

    base::AutoUnlock unlock(lock_);
    NotifyRead(bytes_to_read);
  }

  watchers_.NotifyState(GetHandleSignalsStateNoLock());

  return MOJO_RESULT_OK;
}
