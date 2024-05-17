int WebSocketJob::OnReceivedSpdyResponseHeader(
    const spdy::SpdyHeaderBlock& headers, int status) {
  DCHECK_NE(INITIALIZED, state_);
  if (state_ != CONNECTING)
    return status;
  if (status != OK)
    return status;
  handshake_response_->ParseResponseHeaderBlock(headers, challenge_);

  SaveCookiesAndNotifyHeaderComplete();
  return OK;
}
