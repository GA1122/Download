ClientStub* ConnectionToClient::client_stub() {
  return client_control_sender_.get();
}
