void Automation::WaitForAllTabsToStopLoading(Error** error) {
  std::string error_msg;
  if (!SendWaitForAllTabsToStopLoadingJSONRequest(automation(), &error_msg))
    *error = new Error(kUnknownError, error_msg);
}
