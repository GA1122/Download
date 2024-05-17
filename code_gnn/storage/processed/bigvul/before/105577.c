void Automation::GetAppModalDialogMessage(std::string* message, Error** error) {
  *error = CheckAlertsSupported();
  if (*error)
    return;

  std::string error_msg;
  if (!SendGetAppModalDialogMessageJSONRequest(
          automation(), message, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
