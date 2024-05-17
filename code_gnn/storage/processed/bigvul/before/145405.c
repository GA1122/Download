void P2PQuicTransportImpl::OnPacketDataReceived(const char* data,
                                                size_t data_len) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  quic::QuicReceivedPacket packet(data, data_len, clock_->Now());
  ProcessUdpPacket(connection()->self_address(), connection()->peer_address(),
                   packet);
}
