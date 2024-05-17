  void ExpectConnectionNotEstablished() {
    EXPECT_FALSE(client_peer_->quic_transport()->IsEncryptionEstablished());
    EXPECT_FALSE(client_peer_->quic_transport()->IsCryptoHandshakeConfirmed());
    EXPECT_FALSE(server_peer_->quic_transport()->IsCryptoHandshakeConfirmed());
    EXPECT_FALSE(server_peer_->quic_transport()->IsEncryptionEstablished());
  }
