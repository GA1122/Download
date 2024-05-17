void HWNDMessageHandler::StackAbove(HWND other_hwnd) {
  SetWindowPos(hwnd(), other_hwnd, 0, 0, 0, 0,
               SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
}
