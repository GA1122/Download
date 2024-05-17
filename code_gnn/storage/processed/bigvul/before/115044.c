void TestingAutomationProvider::SignInToSync(Browser* browser,
                                             DictionaryValue* args,
                                             IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string username;
  std::string password;
  if (!args->GetString("username", &username) ||
      !args->GetString("password", &password)) {
      reply.SendError("Invalid or missing args");
      return;
  }
  if (sync_waiter_.get() == NULL) {
    sync_waiter_.reset(new ProfileSyncServiceHarness(
        browser->profile(), username, password));
  } else {
    sync_waiter_->SetCredentials(username, password);
  }
  if (sync_waiter_->SetupSync()) {
    scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
    return_value->SetBoolean("success", true);
    reply.SendSuccess(return_value.get());
  } else {
    reply.SendError("Signing in to sync was unsuccessful");
  }
}
