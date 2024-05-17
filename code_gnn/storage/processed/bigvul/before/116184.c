void SocketStreamDispatcherHost::OnSentData(net::SocketStream* socket,
                                            int amount_sent) {
  int socket_id = SocketStreamHost::SocketIdFromSocketStream(socket);
  DVLOG(1) << "SocketStreamDispatcherHost::OnSentData socket_id=" << socket_id
           << " amount_sent=" << amount_sent;
  if (socket_id == content::kNoSocketId) {
    LOG(ERROR) << "NoSocketId in OnSentData";
    return;
  }
  if (!Send(new SocketStreamMsg_SentData(socket_id, amount_sent))) {
    LOG(ERROR) << "SocketStreamMsg_SentData failed.";
    DeleteSocketStreamHost(socket_id);
  }
}
