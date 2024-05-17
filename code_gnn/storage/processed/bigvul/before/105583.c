Error* Automation::GetIndicesForTab(
    int tab_id, int* browser_index, int* tab_index) {
  std::string error_msg;
  if (!SendGetIndicesFromTabIdJSONRequest(
          automation(), tab_id, browser_index, tab_index, &error_msg)) {
    return new Error(kUnknownError, error_msg);
  }
  return NULL;
}
