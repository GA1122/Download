void WebSocketJob::OnReceivedData(
    SocketStream* socket, const char* data, int len) {
  DCHECK_NE(INITIALIZED, state_);
  if (state_ == CLOSED)
    return;
  if (state_ == CONNECTING) {
    OnReceivedHandshakeResponse(socket, data, len);
    return;
  }
  DCHECK(state_ == OPEN || state_ == CLOSING);
  std::string received_data;
  receive_frame_handler_->AppendData(data, len);
  while (receive_frame_handler_->UpdateCurrentBuffer(false) > 0) {
    received_data +=
        std::string(receive_frame_handler_->GetCurrentBuffer()->data(),
                    receive_frame_handler_->GetCurrentBufferSize());
    receive_frame_handler_->ReleaseCurrentBuffer();
  }
  if (delegate_ && !received_data.empty())
      delegate_->OnReceivedData(
          socket, received_data.data(), received_data.size());
}
