void RTCPeerConnectionHandler::OnInterestingUsage(int usage_pattern) {
  client_->DidNoteInterestingUsage(usage_pattern);
}
