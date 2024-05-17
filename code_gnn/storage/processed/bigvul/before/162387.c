void Core::ClosePeerConnection(uint64_t peer_connection_id) {
  RequestContext request_context;
  GetNodeController()->ClosePeerConnection(peer_connection_id);
}
