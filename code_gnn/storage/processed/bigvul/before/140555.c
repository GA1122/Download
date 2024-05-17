bool SpdyProxyClientSocket::IsConnectedAndIdle() const {
  return IsConnected() && read_buffer_queue_.IsEmpty() &&
      spdy_stream_->IsOpen();
}
