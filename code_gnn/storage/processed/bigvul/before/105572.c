void Automation::CloseTab(int tab_id, Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  std::string error_msg;
  if (!SendCloseTabJSONRequest(automation(), windex, tab_index, &error_msg))
    *error = new Error(kUnknownError, error_msg);
}