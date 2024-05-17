  void ReleaseAndResetDispatchedMessages() {
    ReleaseTouchPoint(0);
    SendTouchEvent();
    MockWidgetInputHandler::MessageVector events =
        GetAndResetDispatchedMessages();
  }
