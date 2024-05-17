  void InvokePaymentRequestWithJs(const std::string& js) {
    ResetEventObserver(DialogEvent::DIALOG_OPENED);

    ASSERT_TRUE(content::ExecuteScript(GetActiveWebContents(), js));

    WaitForObservedEvent();
  }
