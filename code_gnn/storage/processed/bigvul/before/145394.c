P2PQuicStreamImpl* P2PQuicTransportImpl::CreateOutgoingBidirectionalStream() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  P2PQuicStreamImpl* stream = CreateStreamInternal(GetNextOutgoingStreamId());
  ActivateStream(std::unique_ptr<P2PQuicStreamImpl>(stream));
  return stream;
}
