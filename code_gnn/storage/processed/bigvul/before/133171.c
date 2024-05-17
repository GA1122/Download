bool HWNDMessageHandler::SetTitle(const base::string16& title) {
  base::string16 current_title;
  size_t len_with_null = GetWindowTextLength(hwnd()) + 1;
  if (len_with_null == 1 && title.length() == 0)
    return false;
  if (len_with_null - 1 == title.length() &&
      GetWindowText(
          hwnd(), WriteInto(&current_title, len_with_null), len_with_null) &&
      current_title == title)
    return false;
  SetWindowText(hwnd(), title.c_str());
  return true;
}
