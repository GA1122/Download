void TestingAutomationProvider::Login(DictionaryValue* args,
                                      IPC::Message* reply_message) {
  std::string username, password;
  if (!args->GetString("username", &username) ||
      !args->GetString("password", &password)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing args.");
    return;
  }

  chromeos::ExistingUserController* controller =
      chromeos::ExistingUserController::current_controller();

  new LoginObserver(controller, this, reply_message);

  chromeos::WebUILoginDisplay* webui_login_display =
      static_cast<chromeos::WebUILoginDisplay*>(controller->login_display());
  webui_login_display->ShowSigninScreenForCreds(username, password);
}
