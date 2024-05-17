void Automation::AcceptOrDismissAppModalDialog(bool accept, Error** error) {
  *error = CheckAlertsSupported();
  if (*error)
    return;

  std::string error_msg;
  if (!SendAcceptOrDismissAppModalDialogJSONRequest(
          automation(), accept, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
