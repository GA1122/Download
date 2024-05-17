  size_t GetReadSocketCount() const {
    size_t read_sockets = 0;
    for (const auto& socket : sockets_) {
      if (socket.second == SocketStatus::kReadFrom)
        ++read_sockets;
    }
    return read_sockets;
  }
