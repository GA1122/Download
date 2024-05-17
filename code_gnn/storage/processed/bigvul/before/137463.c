void MessageLoopForUI::StartForTesting(
    base::android::JavaMessageHandlerFactory* factory,
    WaitableEvent* test_done_event) {
  static_cast<MessagePumpForUI*>(pump_.get())
      ->StartForUnitTest(this, factory, test_done_event);
}
