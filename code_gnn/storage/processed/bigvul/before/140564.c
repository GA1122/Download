size_t SpdyProxyClientSocket::PopulateUserReadBuffer(char* data, size_t len) {
  return read_buffer_queue_.Dequeue(data, len);
}
