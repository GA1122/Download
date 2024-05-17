void HWNDMessageHandler::Hide() {
  if (IsWindow(hwnd())) {
    SetWindowPos(hwnd(), NULL, 0, 0, 0, 0,
                 SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE |
                 SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);

    if (!GetParent(hwnd()))
      NotifyOwnedWindowsParentClosing();
  }
}
