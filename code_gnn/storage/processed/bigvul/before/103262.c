void WebSocketJob::RestartWithAuth(
    const string16& username,
    const string16& password) {
  state_ = CONNECTING;
  socket_->RestartWithAuth(username, password);
}
