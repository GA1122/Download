void WebSocketJob::OnReceivedHandshakeResponse(
    SocketStream* socket, const char* data, int len) {
  DCHECK_EQ(state_, CONNECTING);
  if (handshake_response_->HasResponse()) {
    receive_frame_handler_->AppendData(data, len);
    return;
  }

  size_t response_length = handshake_response_->ParseRawResponse(data, len);
  if (!handshake_response_->HasResponse()) {
    return;
  }
  socket_->net_log()->AddEvent(
      NetLog::TYPE_WEB_SOCKET_READ_RESPONSE_HEADERS,
      make_scoped_refptr(new NetLogWebSocketHandshakeParameter(
          handshake_response_->GetRawResponse())));
  if (len - response_length > 0) {
    receive_frame_handler_->AppendData(data + response_length,
                                       len - response_length);
  }
  SaveCookiesAndNotifyHeaderComplete();
}
