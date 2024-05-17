  void DoClose(SocketStreamEvent* event) {
    event->socket->Close();
  }
