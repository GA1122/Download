void QuicTransportHost::Stop() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  quic_transport_->Stop();
}
