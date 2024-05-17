  bool SendPacket(QuicPacketSequenceNumber sequence_number,
                  QuicPacket* packet,
                  bool resend,
                  bool force) {
    return QuicConnection::SendPacket(sequence_number, packet, resend, force);
  }
