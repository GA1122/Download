  void InitializeWithFailingProofVerification() {
    Initialize(false);
    std::unique_ptr<quic::ProofVerifier> proof_verifier(
        new FailingProofVerifier);
    std::unique_ptr<quic::QuicCryptoClientConfig> crypto_client_config =
        std::make_unique<quic::QuicCryptoClientConfig>(
            std::move(proof_verifier),
            quic::TlsClientHandshaker::CreateSslCtx());
    client_peer_->quic_transport()->set_crypto_client_config(
        std::move(crypto_client_config));
    client_peer_->quic_transport()->InitializeCryptoStream();
    server_peer_->quic_transport()->InitializeCryptoStream();
  }
