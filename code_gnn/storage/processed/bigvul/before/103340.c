void P2PSocketDispatcherHost::OnCreateSocket(
    const IPC::Message& msg, P2PSocketType type, int socket_id,
    const net::IPEndPoint& local_address,
    const net::IPEndPoint& remote_address) {
  if (LookupSocket(msg.routing_id(), socket_id)) {
    LOG(ERROR) << "Received P2PHostMsg_CreateSocket for socket "
        "that already exists.";
    return;
  }

  scoped_ptr<P2PSocketHost> socket(
      P2PSocketHost::Create(this, msg.routing_id(), socket_id, type));

  if (!socket.get()) {
    Send(new P2PMsg_OnError(msg.routing_id(), socket_id));
    return;
  }

  if (socket->Init(local_address, remote_address)) {
    sockets_.insert(std::pair<ExtendedSocketId, P2PSocketHost*>(
        ExtendedSocketId(msg.routing_id(), socket_id), socket.release()));
  }
}
