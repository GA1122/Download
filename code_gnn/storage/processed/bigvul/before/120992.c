  void SetOnClose(const base::Callback<void(SocketStreamEvent*)>& callback) {
    on_close_ = callback;
  }
