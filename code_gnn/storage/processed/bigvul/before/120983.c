  void DoRestartWithAuth(SocketStreamEvent* event) {
    VLOG(1) << "RestartWithAuth username=" << credentials_.username()
            << " password=" << credentials_.password();
    event->socket->RestartWithAuth(credentials_);
  }
