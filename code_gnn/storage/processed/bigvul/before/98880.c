void WebSocket::ProcessFrameData() {
  DCHECK(current_read_buf_);
  const char* start_frame =
      current_read_buf_->StartOfBuffer() + read_consumed_len_;
  const char* next_frame = start_frame;
  const char* p = next_frame;
  const char* end =
      current_read_buf_->StartOfBuffer() + current_read_buf_->offset();
  while (p < end) {
    unsigned char frame_byte = static_cast<unsigned char>(*p++);
    if ((frame_byte & 0x80) == 0x80) {
      int length = 0;
      while (p < end) {
        if (length > std::numeric_limits<int>::max() / 128) {
          socket_stream_->Close();
          return;
        }
        unsigned char c = static_cast<unsigned char>(*p);
        length = length * 128 + (c & 0x7f);
        ++p;
        if ((c & 0x80) != 0x80)
          break;
      }
      if (p + length < end) {
        p += length;
        next_frame = p;
      } else {
        break;
      }
    } else {
      const char* msg_start = p;
      while (p < end && *p != '\xff')
        ++p;
      if (p < end && *p == '\xff') {
        if (frame_byte == 0x00 && delegate_)
          delegate_->OnMessage(this, std::string(msg_start, p - msg_start));
        ++p;
        next_frame = p;
      }
    }
  }
  SkipReadBuffer(next_frame - start_frame);
}
