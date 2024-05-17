int HttpProxyClientSocket::PrepareForAuthRestart() {
  if (!response_.headers.get())
    return ERR_CONNECTION_RESET;

  bool keep_alive = false;
  if (response_.headers->IsKeepAlive() &&
      http_stream_parser_->CanFindEndOfResponse()) {
    if (!http_stream_parser_->IsResponseBodyComplete()) {
      next_state_ = STATE_DRAIN_BODY;
      drain_buf_ = new IOBuffer(kDrainBodyBufferSize);
      return OK;
    }
    keep_alive = true;
  }

  return DidDrainBodyForAuthRestart(keep_alive);
}
