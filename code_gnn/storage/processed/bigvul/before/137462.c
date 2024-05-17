void MessageLoopForUI::Start() {
  static_cast<MessagePumpForUI*>(pump_.get())->Start(this);
}
