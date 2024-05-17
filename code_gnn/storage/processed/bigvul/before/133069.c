void HWNDMessageHandler::CloseNow() {
  waiting_for_close_now_ = false;
  if (IsWindow(hwnd()))
    DestroyWindow(hwnd());
}
