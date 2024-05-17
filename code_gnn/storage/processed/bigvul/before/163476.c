void QuicClientPromisedInfo::Reset(QuicRstStreamErrorCode error_code) {
  QuicClientPushPromiseIndex::Delegate* delegate = client_request_delegate_;
  session_->ResetPromised(id_, error_code);
  session_->DeletePromised(this);
  if (delegate) {
    delegate->OnRendezvousResult(nullptr);
  }
}
