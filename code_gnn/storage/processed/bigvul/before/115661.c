void ClientSession::InjectKeyEvent(const KeyEvent& event) {
  if (authenticated_ && !ShouldIgnoreRemoteKeyboardInput(event)) {
    RecordKeyEvent(event);
    input_stub_->InjectKeyEvent(event);
  }
}
