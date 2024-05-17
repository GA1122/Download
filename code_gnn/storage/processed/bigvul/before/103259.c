void WebSocketJob::OnSentSpdyData(int amount_sent) {
  DCHECK_NE(INITIALIZED, state_);
  DCHECK_NE(CONNECTING, state_);
  if (state_ == CLOSED)
    return;
  if (!spdy_websocket_stream_.get())
    return;
  OnSentData(socket_, amount_sent);
}
