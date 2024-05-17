void AutomationProvider::OnUnhandledMessage() {
  LOG(ERROR) << "AutomationProvider received a message it can't handle. "
             << "Please make sure that you use switches::kTestingChannelID "
             << "for test code (TestingAutomationProvider), and "
             << "switches::kAutomationClientChannelID for everything else "
             << "(like ChromeFrame). Closing the automation channel.";
  channel_->Close();
}
