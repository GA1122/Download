P2PQuicStreamImpl* P2PQuicTransportImpl::CreateIncomingStream(
    quic::QuicStreamId id) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  P2PQuicStreamImpl* stream = CreateStreamInternal(id);
  ActivateStream(std::unique_ptr<P2PQuicStreamImpl>(stream));
  delegate_->OnStream(stream);
  return stream;
}
