  QuicPeerForTest(
      std::unique_ptr<FakePacketTransport> packet_transport,
      std::unique_ptr<MockP2PQuicTransportDelegate> quic_transport_delegate,
      std::unique_ptr<P2PQuicTransportImpl> quic_transport,
      rtc::scoped_refptr<rtc::RTCCertificate> certificate)
      : packet_transport_(std::move(packet_transport)),
        quic_transport_delegate_(std::move(quic_transport_delegate)),
        quic_transport_(std::move(quic_transport)),
        certificate_(certificate) {}
