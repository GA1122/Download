RTCPeerConnectionHandler::FindTransceiver(uintptr_t id) {
  for (auto it = rtp_transceivers_.begin(); it != rtp_transceivers_.end();
       ++it) {
    if ((*it)->Id() == id)
      return it;
  }
  return rtp_transceivers_.end();
}
