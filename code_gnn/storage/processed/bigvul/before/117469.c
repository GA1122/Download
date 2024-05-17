void TestingAutomationProvider::GetLoginInfo(DictionaryValue* args,
                                             IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);

  const UserManager* user_manager = UserManager::Get();
  if (!user_manager)
    reply.SendError("No user manager!");
  const chromeos::ScreenLocker* screen_locker =
      chromeos::ScreenLocker::default_screen_locker();

  return_value->SetString("login_ui_type", "webui");
  return_value->SetBoolean("is_owner", user_manager->current_user_is_owner());
  return_value->SetBoolean("is_logged_in", user_manager->user_is_logged_in());
  return_value->SetBoolean("is_screen_locked", screen_locker);
  if (user_manager->user_is_logged_in()) {
    return_value->SetBoolean("is_guest", user_manager->IsLoggedInAsGuest());
    return_value->SetString("email", user_manager->logged_in_user().email());
  }

  reply.SendSuccess(return_value.get());
}
