void WebSocketJob::OnSentHandshakeRequest(
    SocketStream* socket, int amount_sent) {
  DCHECK_EQ(state_, CONNECTING);
  handshake_request_sent_ += amount_sent;
  DCHECK_LE(handshake_request_sent_, handshake_request_->raw_length());
  if (handshake_request_sent_ >= handshake_request_->raw_length()) {
    if (delegate_)
      delegate_->OnSentData(
          socket,
          handshake_request_->original_length());
    handshake_request_.reset();
  }
}
