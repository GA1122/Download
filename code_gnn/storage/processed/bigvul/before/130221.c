bool ClipboardWin::HandleMessage(
    UINT message, WPARAM wparam, LPARAM lparam, LRESULT* result) {
  if (message == WM_CLIPBOARDUPDATE) {
    OnClipboardUpdate();
    *result = 0;
    return true;
  }

  return false;
}
