void Automation::MouseClick(int tab_id,
                            const gfx::Point& p,
                            automation::MouseButton button,
                            Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  std::string error_msg;
  if (!SendMouseClickJSONRequest(
          automation(), windex, tab_index, button, p.x(), p.y(), &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
