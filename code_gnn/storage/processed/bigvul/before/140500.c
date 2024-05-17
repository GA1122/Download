int HttpProxyClientSocket::DoDrainBody() {
  DCHECK(drain_buf_.get());
  DCHECK(transport_->is_initialized());
  next_state_ = STATE_DRAIN_BODY_COMPLETE;
  return http_stream_parser_->ReadResponseBody(
      drain_buf_.get(), kDrainBodyBufferSize, io_callback_);
}
