  void SetOnError(const base::Callback<void(SocketStreamEvent*)>& callback) {
    on_error_ = callback;
  }
