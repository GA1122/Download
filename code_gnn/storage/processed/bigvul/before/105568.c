void Automation::CaptureEntirePageAsPNG(int tab_id,
                                        const FilePath& path,
                                        Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  std::string error_msg;
  if (!SendCaptureEntirePageJSONRequest(
          automation(), windex, tab_index, path, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
