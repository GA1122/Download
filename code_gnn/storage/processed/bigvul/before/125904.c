bool FramerVisitorCapturingAcks::OnPacketHeader(
    const QuicPacketHeader& header) {
  header_ = header;
  return true;
}
