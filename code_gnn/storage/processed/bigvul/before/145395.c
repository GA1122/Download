P2PQuicStreamImpl* P2PQuicTransportImpl::CreateStream() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  return CreateOutgoingBidirectionalStream();
}
