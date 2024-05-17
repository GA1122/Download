bool HpackDecoder::HandleControlFrameHeadersData(SpdyStreamId id,
                                                 const char* headers_data,
                                                 size_t headers_data_length) {
  decoded_block_.clear();

  size_t new_size = headers_block_buffer_.size() + headers_data_length;
  if (new_size > kMaxDecodeBufferSize) {
    return false;
  }
  headers_block_buffer_.insert(headers_block_buffer_.end(),
                               headers_data,
                               headers_data + headers_data_length);
  return true;
}
