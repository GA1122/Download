void WebSocket::OnSentData(SocketStream* socket_stream, int amount_sent) {
  DCHECK(socket_stream == socket_stream_);
  DCHECK(current_write_buf_);
  current_write_buf_->DidConsume(amount_sent);
  DCHECK_GE(current_write_buf_->BytesRemaining(), 0);
  if (current_write_buf_->BytesRemaining() == 0) {
    current_write_buf_ = NULL;
    pending_write_bufs_.pop_front();
  }
  origin_loop_->PostTask(FROM_HERE,
                         NewRunnableMethod(this, &WebSocket::SendPending));
}
