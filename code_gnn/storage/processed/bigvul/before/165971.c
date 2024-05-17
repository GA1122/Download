RTCPeerConnectionHandler::FindReceiver(uintptr_t id) {
  for (auto it = rtp_receivers_.begin(); it != rtp_receivers_.end(); ++it) {
    if ((*it)->Id() == id)
      return it;
  }
  return rtp_receivers_.end();
}
