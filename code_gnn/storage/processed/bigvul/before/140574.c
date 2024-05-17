bool SpdyProxyClientSocket::WasEverUsed() const {
  return was_ever_used_ || (spdy_stream_.get() && spdy_stream_->WasEverUsed());
}
