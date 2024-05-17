  void SetOnSentData(
      const base::Callback<void(SocketStreamEvent*)>& callback) {
    on_sent_data_ = callback;
  }
