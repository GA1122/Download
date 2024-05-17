void P2PSocketDispatcherHost::OnDestroySocket(const IPC::Message& msg,
                                              int socket_id) {
  SocketsMap::iterator it = sockets_.find(
      ExtendedSocketId(msg.routing_id(), socket_id));
  if (it != sockets_.end()) {
    delete it->second;
    sockets_.erase(it);
  } else {
    LOG(ERROR) << "Received P2PHostMsg_DestroySocket for invalid socket_id.";
  }
}
