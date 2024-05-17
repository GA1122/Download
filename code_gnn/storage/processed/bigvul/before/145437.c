  quic::QuicConnection* client_connection() {
    return client_peer_->quic_transport()->connection();
  }
