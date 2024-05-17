void Automation::MouseDrag(int tab_id,
                           const gfx::Point& start,
                           const gfx::Point& end,
                           Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  std::string error_msg;
  if (!SendMouseDragJSONRequest(automation(), windex, tab_index, start.x(),
                                start.y(), end.x(), end.y(), &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
  }
}
