void Automation::AcceptPromptAppModalDialog(const std::string& prompt_text,
                                            Error** error) {
  *error = CheckAlertsSupported();
  if (*error)
    return;

  std::string error_msg;
  if (!SendAcceptPromptAppModalDialogJSONRequest(
          automation(), prompt_text, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
