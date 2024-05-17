QuicAsyncStatus QuicClientPromisedInfo::HandleClientRequest(
    const SpdyHeaderBlock& request_headers,
    QuicClientPushPromiseIndex::Delegate* delegate) {
  if (session_->IsClosedStream(id_)) {
    session_->DeletePromised(this);
    return QUIC_FAILURE;
  }

  if (is_validating()) {
    return QUIC_FAILURE;
  }

  client_request_delegate_ = delegate;
  client_request_headers_.reset(new SpdyHeaderBlock(request_headers.Clone()));
  if (!response_headers_) {
    return QUIC_PENDING;
  }
  return FinalValidation();
}
