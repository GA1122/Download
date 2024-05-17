void Automation::SendWebKeyEvent(int tab_id,
                                 const WebKeyEvent& key_event,
                                 Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  std::string error_msg;
  if (!SendWebKeyEventJSONRequest(
          automation(), windex, tab_index, key_event, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
