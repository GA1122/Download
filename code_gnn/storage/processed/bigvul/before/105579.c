void Automation::GetChromeDriverAutomationVersion(int* version, Error** error) {
  std::string error_msg;
  if (!SendGetChromeDriverAutomationVersion(automation(), version, &error_msg))
    *error = new Error(kUnknownError, error_msg);
}
