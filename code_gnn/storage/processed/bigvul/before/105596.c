void Automation::SendNativeKeyEvent(int tab_id,
                                    ui::KeyboardCode key_code,
                                    int modifiers,
                                    Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  std::string error_msg;
  if (!SendNativeKeyEventJSONRequest(
         automation(), windex, tab_index, key_code, modifiers, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
