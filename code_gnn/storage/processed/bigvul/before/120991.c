  void SetOnAuthRequired(
      const base::Callback<void(SocketStreamEvent*)>& callback) {
    on_auth_required_ = callback;
  }
