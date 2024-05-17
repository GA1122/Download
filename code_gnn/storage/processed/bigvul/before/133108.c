void HWNDMessageHandler::OnAppbarAutohideEdgesChanged() {
  RECT client;
  GetWindowRect(hwnd(), &client);
  SetWindowPos(hwnd(), NULL, client.left, client.top,
               client.right - client.left, client.bottom - client.top,
               SWP_FRAMECHANGED);
}
