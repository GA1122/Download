bool BitReaderCore::Refill(int min_nbits) {
  DCHECK_LE(min_nbits, kRegWidthInBits);

  RefillCurrentRegister();
  if (min_nbits <= nbits_)
    return true;
  DCHECK_EQ(nbits_next_, 0);
  DCHECK_EQ(reg_next_, 0u);

  int max_nbytes = sizeof(reg_next_);

  const uint8_t* byte_stream_window;
  int window_size =
      byte_stream_provider_->GetBytes(max_nbytes, &byte_stream_window);
  DCHECK_GE(window_size, 0);
  DCHECK_LE(window_size, max_nbytes);
  if (window_size == 0)
    return false;

  reg_next_ = 0;
  memcpy(&reg_next_, byte_stream_window, window_size);
  reg_next_ = base::NetToHost64(reg_next_);
  nbits_next_ = window_size * 8;

  RefillCurrentRegister();

  return (nbits_ >= min_nbits);
}
