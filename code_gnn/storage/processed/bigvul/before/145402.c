bool P2PQuicTransportImpl::IsClosed() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  return !connection_->connected();
}
