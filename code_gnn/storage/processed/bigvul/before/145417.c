  void DisconnectPeerTransport(FakePacketTransport* peer_packet_transport) {
    DCHECK(peer_packet_transport_ == peer_packet_transport);
    peer_packet_transport_ = nullptr;
  }
