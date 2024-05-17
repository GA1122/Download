  void Reset() {
    ui::MockIMEInputContextHandler::Reset();
    send_key_event_call_count_ = 0;
  }
