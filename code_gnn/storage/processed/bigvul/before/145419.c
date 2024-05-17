  void ExpectStreamsClosed() {
    EXPECT_EQ(0u, client_peer_->quic_transport()->GetNumActiveStreams());
    EXPECT_TRUE(client_peer_->quic_transport()->IsClosedStream(
        client_peer()->stream_id()));

    EXPECT_EQ(0u, server_peer_->quic_transport()->GetNumActiveStreams());
    EXPECT_TRUE(server_peer()->quic_transport()->IsClosedStream(
        server_peer()->stream_id()));
  }
