void P2PQuicStreamImpl::OnFinRead() {
  DCHECK(delegate_);
  quic::QuicStream::OnFinRead();
  delegate_->OnRemoteFinish();
}
