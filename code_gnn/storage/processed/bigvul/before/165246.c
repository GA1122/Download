  void OnClientConnectionLost() {
    client_.reset();
    binding_.Close();
  }
