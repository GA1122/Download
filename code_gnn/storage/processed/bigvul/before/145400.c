 void P2PQuicTransportImpl::InitializeCryptoStream() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!crypto_stream_);
  switch (perspective_) {
    case quic::Perspective::IS_CLIENT: {
      if (!crypto_client_config_) {
        std::unique_ptr<quic::ProofVerifier> proof_verifier(
            new InsecureProofVerifier);
        crypto_client_config_ = std::make_unique<quic::QuicCryptoClientConfig>(
            std::move(proof_verifier),
            quic::TlsClientHandshaker::CreateSslCtx());
      }
      char random_hostname[kHostnameLength];
      helper_->GetRandomGenerator()->RandBytes(random_hostname,
                                               kHostnameLength);
      quic::QuicServerId server_id(
           quic::QuicString(random_hostname, kHostnameLength),
           0,
           false);
      crypto_stream_ = std::make_unique<quic::QuicCryptoClientStream>(
          server_id,  this,
          crypto_client_config_->proof_verifier()->CreateDefaultContext(),
          crypto_client_config_.get(),  this);
      QuicSession::Initialize();
      break;
    }
    case quic::Perspective::IS_SERVER: {
      std::unique_ptr<quic::ProofSource> proof_source(new DummyProofSource);
      crypto_server_config_ = std::make_unique<quic::QuicCryptoServerConfig>(
          quic::QuicCryptoServerConfig::TESTING, helper_->GetRandomGenerator(),
          std::move(proof_source), quic::KeyExchangeSource::Default(),
          quic::TlsServerHandshaker::CreateSslCtx());
      quic::QuicCryptoServerConfig::ConfigOptions options;
      std::unique_ptr<quic::CryptoHandshakeMessage> message(
          crypto_server_config_->AddDefaultConfig(
              helper_->GetRandomGenerator(), helper_->GetClock(), options));
      compressed_certs_cache_.reset(new quic::QuicCompressedCertsCache(
          quic::QuicCompressedCertsCache::kQuicCompressedCertsCacheSize));
      bool use_stateless_rejects_if_peer_supported = false;
      server_stream_helper_ = std::make_unique<DummyCryptoServerStreamHelper>(
          helper_->GetRandomGenerator());

      crypto_stream_ = std::make_unique<quic::QuicCryptoServerStream>(
          crypto_server_config_.get(), compressed_certs_cache_.get(),
          use_stateless_rejects_if_peer_supported, this,
          server_stream_helper_.get());
      QuicSession::Initialize();
      break;
    }
    default:
      NOTREACHED();
      break;
  }
}
