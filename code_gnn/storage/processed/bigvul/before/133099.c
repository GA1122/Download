bool IsTopLevelWindow(HWND window) {
  long style = ::GetWindowLong(window, GWL_STYLE);
  if (!(style & WS_CHILD))
    return true;
  HWND parent = ::GetParent(window);
  return !parent || (parent == ::GetDesktopWindow());
}
