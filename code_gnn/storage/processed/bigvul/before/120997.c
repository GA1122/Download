  void SetOnStartOpenConnection(
      const base::Callback<int(SocketStreamEvent*)>& callback) {
    on_start_open_connection_ = callback;
  }
