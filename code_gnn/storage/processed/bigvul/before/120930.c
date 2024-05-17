void SocketStream::DoClose() {
  closing_ = true;
  if (next_state_ == STATE_TCP_CONNECT || next_state_ == STATE_AUTH_REQUIRED) {
    DoLoop(ERR_ABORTED);
    return;
  }
  if (next_state_ == STATE_READ_WRITE && !current_write_buf_.get())
    DoLoop(ERR_ABORTED);

}
