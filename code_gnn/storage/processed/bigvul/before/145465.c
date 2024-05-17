void QuicTransportHost::Start(
    std::vector<std::unique_ptr<rtc::SSLFingerprint>> remote_fingerprints) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  quic_transport_->Start(std::move(remote_fingerprints));
}
