  void AcceptedSocketWithPort(uint16_t port) {
    EXPECT_FALSE(base::Contains(sockets_, port));
    sockets_[port] = SocketStatus::kAccepted;
    CheckAccepted();
    first_accept_loop_.Quit();
  }
