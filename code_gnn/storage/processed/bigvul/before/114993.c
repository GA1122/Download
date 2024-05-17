void TestingAutomationProvider::OnChannelError() {
  if (!reinitialize_on_channel_error_ &&
      browser_shutdown::GetShutdownType() == browser_shutdown::NOT_VALID)
    BrowserList::AttemptExit();
  AutomationProvider::OnChannelError();
}
