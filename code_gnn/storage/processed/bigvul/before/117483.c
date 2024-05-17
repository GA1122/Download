void TestingAutomationProvider::LoginAsGuest(DictionaryValue* args,
                                             IPC::Message* reply_message) {
  chromeos::ExistingUserController* controller =
      chromeos::ExistingUserController::current_controller();
  AutomationJSONReply(this, reply_message).SendSuccess(NULL);
  controller->LoginAsGuest();
}
