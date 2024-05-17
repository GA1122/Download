void HWNDMessageHandler::ShowMaximizedWithBounds(const gfx::Rect& bounds) {
  WINDOWPLACEMENT placement = { 0 };
  placement.length = sizeof(WINDOWPLACEMENT);
  placement.showCmd = SW_SHOWMAXIMIZED;
  placement.rcNormalPosition = bounds.ToRECT();
  SetWindowPlacement(hwnd(), &placement);
}
