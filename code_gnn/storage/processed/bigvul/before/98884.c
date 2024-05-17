WebSocket::WebSocket(Request* request, WebSocketDelegate* delegate)
    : ready_state_(INITIALIZED),
      request_(request),
      handshake_(NULL),
      delegate_(delegate),
      origin_loop_(MessageLoop::current()),
      socket_stream_(NULL),
      max_pending_send_allowed_(0),
      current_read_buf_(NULL),
      read_consumed_len_(0),
      current_write_buf_(NULL) {
  DCHECK(request_.get());
  DCHECK(delegate_);
  DCHECK(origin_loop_);
}
