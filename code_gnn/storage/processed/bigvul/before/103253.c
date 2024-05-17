void WebSocketJob::OnCreatedSpdyStream(int result) {
  DCHECK(spdy_websocket_stream_.get());
  DCHECK(socket_.get());
  DCHECK_NE(ERR_IO_PENDING, result);

  if (state_ == CLOSED) {
    result = ERR_ABORTED;
  } else if (result == OK) {
    state_ = CONNECTING;
    result = ERR_PROTOCOL_SWITCHED;
  } else {
    spdy_websocket_stream_.reset();
  }

  CompleteIO(result);
}
