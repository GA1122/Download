void TestingAutomationProvider::GetSavedPasswords(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  PasswordStore* password_store =
      browser->profile()->GetPasswordStore(Profile::EXPLICIT_ACCESS);

  if (password_store == NULL) {
    AutomationJSONReply reply(this, reply_message);
    reply.SendError("Unable to get password store.");
    return;
  }
  password_store->GetAutofillableLogins(
      new AutomationProviderGetPasswordsObserver(this, reply_message));
}
