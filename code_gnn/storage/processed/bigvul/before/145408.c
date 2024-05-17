void P2PQuicTransportImpl::set_crypto_client_config(
    std::unique_ptr<quic::QuicCryptoClientConfig> crypto_client_config) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  crypto_client_config_ = std::move(crypto_client_config);
}
