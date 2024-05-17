bool WebSocketJob::SendData(const char* data, int len) {
  switch (state_) {
    case INITIALIZED:
      return false;

    case CONNECTING:
      return SendHandshakeRequest(data, len);

    case OPEN:
      {
        send_frame_handler_->AppendData(data, len);
        int err = 0;
        if (!send_frame_handler_->GetCurrentBuffer() &&
            (err = send_frame_handler_->UpdateCurrentBuffer(false)) > 0) {
          DCHECK(!current_buffer_);
          current_buffer_ = new DrainableIOBuffer(
              send_frame_handler_->GetCurrentBuffer(),
              send_frame_handler_->GetCurrentBufferSize());
          return SendDataInternal(
              current_buffer_->data(), current_buffer_->BytesRemaining());
        }
        return err >= 0;
      }

    case CLOSING:
    case CLOSED:
      return false;
  }
  return false;
}
