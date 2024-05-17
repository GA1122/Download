  virtual int WritePacketToWire(QuicPacketSequenceNumber number,
                                const QuicEncryptedPacket& packet,
                                bool resend,
                                int* error) {
    QuicFramer framer(QuicDecrypter::Create(kNULL),
                      QuicEncrypter::Create(kNULL));
    FramerVisitorCapturingAcks visitor;
    framer.set_visitor(&visitor);
    EXPECT_TRUE(framer.ProcessPacket(IPEndPoint(), IPEndPoint(), packet));
    header_ = *visitor.header();
    return packet.length();
  }
