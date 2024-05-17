void QuicClientPromisedInfo::Cancel() {
  client_request_delegate_ = nullptr;
  Reset(QUIC_STREAM_CANCELLED);
}
