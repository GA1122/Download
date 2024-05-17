void ConnectionToClient::CloseOnError() {
  CloseChannels();
  handler_->OnConnectionFailed(this);
}
