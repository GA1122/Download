void TestingAutomationProvider::RemoveSavedPassword(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  DictionaryValue* password_dict = NULL;

  if (!args->GetDictionary("password", &password_dict)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Must specify a password dictionary.");
    return;
  }

  if (!password_dict->HasKey("signon_realm")) {
    AutomationJSONReply(this, reply_message).SendError(
        "Password must include a value for 'signon_realm.'");
    return;
  }
  webkit::forms::PasswordForm to_remove =
      GetPasswordFormFromDict(*password_dict);

  PasswordStore* password_store =
      browser->profile()->GetPasswordStore(Profile::EXPLICIT_ACCESS);
  if (password_store == NULL) {
    AutomationJSONReply(this, reply_message).SendError(
        "Unable to get password store.");
    return;
  }

  PasswordStoreLoginsChangedObserver* observer =
      new PasswordStoreLoginsChangedObserver(
          this, reply_message, PasswordStoreChange::REMOVE, "");
  observer->Init();

  password_store->RemoveLogin(to_remove);
}
