void P2PQuicStreamImpl::OnClose() {
  closed_ = true;
  quic::QuicStream::OnClose();
}
