int HttpProxyClientSocket::Read(IOBuffer* buf, int buf_len,
                                const CompletionCallback& callback) {
  DCHECK(user_callback_.is_null());
  if (next_state_ != STATE_DONE) {
    DCHECK_EQ(407, response_.headers->response_code());
    LogBlockedTunnelResponse();

    return ERR_TUNNEL_CONNECTION_FAILED;
  }

  return transport_->socket()->Read(buf, buf_len, callback);
}
