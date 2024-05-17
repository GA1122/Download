void TestingAutomationProvider::OmniboxAcceptInput(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  NavigationController& controller =
      browser->GetSelectedWebContents()->GetController();
  new OmniboxAcceptNotificationObserver(&controller, this, reply_message);
  browser->window()->GetLocationBar()->AcceptInput();
}
