bool WebSocketJob::SendHandshakeRequest(const char* data, int len) {
  DCHECK_EQ(state_, CONNECTING);
  if (started_to_send_handshake_request_)
    return false;
  if (!handshake_request_->ParseRequest(data, len))
    return false;

  handshake_response_->set_protocol_version(
      handshake_request_->protocol_version());
  AddCookieHeaderAndSend();
  started_to_send_handshake_request_ = true;
  return true;
}
