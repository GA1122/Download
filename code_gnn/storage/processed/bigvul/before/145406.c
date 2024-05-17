void P2PQuicTransportImpl::Start(
    std::vector<std::unique_ptr<rtc::SSLFingerprint>> remote_fingerprints) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK_EQ(remote_fingerprints_.size(), 0u);
  DCHECK_GT(remote_fingerprints.size(), 0u);
  if (IsClosed()) {
    return;
  }
  remote_fingerprints_ = std::move(remote_fingerprints);

  if (perspective_ == quic::Perspective::IS_CLIENT) {
    quic::QuicCryptoClientStream* client_crypto_stream =
        static_cast<quic::QuicCryptoClientStream*>(crypto_stream_.get());
    client_crypto_stream->CryptoConnect();
  }
}
