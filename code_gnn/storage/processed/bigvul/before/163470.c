QuicAsyncStatus QuicClientPromisedInfo::FinalValidation() {
  if (!client_request_delegate_->CheckVary(
          *client_request_headers_, *request_headers_, *response_headers_)) {
    Reset(QUIC_PROMISE_VARY_MISMATCH);
    return QUIC_FAILURE;
  }
  QuicSpdyStream* stream = session_->GetPromisedStream(id_);
  if (!stream) {
    QUIC_BUG << "missing promised stream" << id_;
  }
  QuicClientPushPromiseIndex::Delegate* delegate = client_request_delegate_;
  session_->DeletePromised(this);
  if (delegate) {
    delegate->OnRendezvousResult(stream);
  }
  return QUIC_SUCCESS;
}
