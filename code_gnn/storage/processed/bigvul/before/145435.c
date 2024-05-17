  void StartHandshake() {
    std::vector<std::unique_ptr<rtc::SSLFingerprint>> server_fingerprints;
    server_fingerprints.emplace_back(rtc::SSLFingerprint::Create(
        "sha-256", server_peer_->certificate()->identity()));
    server_peer_->quic_transport()->Start(std::move(server_fingerprints));

    std::vector<std::unique_ptr<rtc::SSLFingerprint>> client_fingerprints;
    client_fingerprints.emplace_back(rtc::SSLFingerprint::Create(
        "sha-256", client_peer_->certificate()->identity()));
    client_peer_->quic_transport()->Start(std::move(client_fingerprints));
  }
