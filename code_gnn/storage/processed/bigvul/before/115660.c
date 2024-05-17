ClientSession::ClientSession(
    EventHandler* event_handler,
    scoped_refptr<protocol::ConnectionToClient> connection,
    protocol::InputStub* input_stub,
    Capturer* capturer)
    : event_handler_(event_handler),
      connection_(connection),
      client_jid_(connection->session()->jid()),
      input_stub_(input_stub),
      capturer_(capturer),
      authenticated_(false),
      awaiting_continue_approval_(false),
      remote_mouse_button_state_(0) {
  connection_->SetEventHandler(this);

  connection_->set_host_stub(this);
  connection_->set_input_stub(this);
}
