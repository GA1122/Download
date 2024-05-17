  void ExpectTransportsClosed() {
    EXPECT_TRUE(client_peer_->quic_transport()->IsClosed());
    EXPECT_TRUE(server_peer_->quic_transport()->IsClosed());
  }
