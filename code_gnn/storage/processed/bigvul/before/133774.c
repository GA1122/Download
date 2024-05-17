void SSLClientSocketOpenSSL::OnRecvComplete(int result) {
  if (next_handshake_state_ == STATE_HANDSHAKE) {
    OnHandshakeIOComplete(result);
    return;
  }

  if (!user_read_buf_.get())
    return;

  int rv = DoReadLoop(result);
  if (rv != ERR_IO_PENDING)
    DoReadCallback(rv);
}
