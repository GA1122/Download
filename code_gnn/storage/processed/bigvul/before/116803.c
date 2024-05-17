int WebRTCTransportImpl::SendRTCPPacket(int channel, const void* data,
                                        int len) {
  return network_->ReceivedRTCPPacket(channel, data, len);
}
