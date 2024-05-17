void TestingAutomationProvider::UnlockScreen(DictionaryValue* args,
                                             IPC::Message* reply_message) {
  std::string password;
  if (!args->GetString("password", &password)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing args.");
    return;
  }

  chromeos::ScreenLocker* screen_locker =
      chromeos::ScreenLocker::default_screen_locker();
  if (!screen_locker) {
    AutomationJSONReply(this, reply_message).SendError(
        "No default screen locker. Are you sure the screen is locked?");
    return;
  }

  new ScreenUnlockObserver(this, reply_message);
  screen_locker->Authenticate(ASCIIToUTF16(password));
}
