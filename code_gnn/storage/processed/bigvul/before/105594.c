void Automation::NavigateToURL(int tab_id,
                               const std::string& url,
                               Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  AutomationMsg_NavigationResponseValues navigate_response;
  std::string error_msg;
  if (!SendNavigateToURLJSONRequest(automation(), windex, tab_index,
                                    GURL(url), 1, &navigate_response,
                                    &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
    return;
  }
  if (navigate_response == AUTOMATION_MSG_NAVIGATION_ERROR)
    *error = new Error(kUnknownError, "Navigation error occurred");
}
