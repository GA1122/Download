void WebSocket::DoReceivedData() {
  DCHECK(MessageLoop::current() == origin_loop_);
  switch (ready_state_) {
    case CONNECTING:
      {
        DCHECK(handshake_.get());
        DCHECK(current_read_buf_);
        const char* data =
            current_read_buf_->StartOfBuffer() + read_consumed_len_;
        size_t len = current_read_buf_->offset() - read_consumed_len_;
        int eoh = handshake_->ReadServerHandshake(data, len);
        if (eoh < 0) {
          return;
        }
        SkipReadBuffer(eoh);
      }
      if (handshake_->mode() != WebSocketHandshake::MODE_CONNECTED) {
        socket_stream_->Close();
        return;
      }
      ready_state_ = OPEN;
      if (delegate_)
        delegate_->OnOpen(this);
      if (current_read_buf_->offset() == read_consumed_len_) {
        break;
      }
    case OPEN:
      ProcessFrameData();
      break;

    case CLOSED:
      break;
    default:
      NOTREACHED();
      break;
  }
}
