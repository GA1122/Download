void TestingAutomationProvider::EnrollEnterpriseDevice(
    DictionaryValue* args, IPC::Message* reply_message) {
  std::string user, password;
  if (!args->GetString("user", &user) ||
      !args->GetString("password", &password)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Invalid or missing args.");
    return;
  }
  chromeos::ExistingUserController* user_controller =
      chromeos::ExistingUserController::current_controller();
  if (!user_controller) {
    AutomationJSONReply(this, reply_message).SendError(
        "Unable to access ExistingUserController");
    return;
  }
  user_controller->login_display_host()->StartWizard(
      chromeos::WizardController::kEnterpriseEnrollmentScreenName,
      NULL);
  chromeos::WizardController* wizard_controller =
      chromeos::WizardController::default_controller();
  if (!wizard_controller) {
    AutomationJSONReply(this, reply_message).SendError(
        "Unable to access WizardController");
    return;
  }
  chromeos::EnterpriseEnrollmentScreen* enroll_screen =
      wizard_controller->GetEnterpriseEnrollmentScreen();
  if (!enroll_screen) {
    AutomationJSONReply(this, reply_message).SendError(
        "Unable to access EnterpriseEnrollmentScreen");
    return;
  }
  new EnrollmentObserver(this, reply_message, enroll_screen->GetActor(),
                         enroll_screen);
  enroll_screen->GetActor()->SubmitTestCredentials(user, password);
}
