void TestingAutomationProvider::OpenNewBrowserWindowWithNewProfile(
    base::DictionaryValue* args, IPC::Message* reply_message) {
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  new BrowserOpenedWithNewProfileNotificationObserver(this, reply_message);
  profile_manager->CreateMultiProfileAsync();
}
