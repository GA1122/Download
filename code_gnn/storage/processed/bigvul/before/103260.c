void WebSocketJob::OnSentSpdyHeaders(int result) {
  DCHECK_NE(INITIALIZED, state_);
  if (state_ != CONNECTING)
    return;
  if (delegate_)
    delegate_->OnSentData(socket_, handshake_request_->original_length());
  handshake_request_.reset();
}
