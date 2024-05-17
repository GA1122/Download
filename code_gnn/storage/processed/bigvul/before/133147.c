void HWNDMessageHandler::OnWindowPosChanged(WINDOWPOS* window_pos) {
  if (DidClientAreaSizeChange(window_pos))
    ClientAreaSizeChanged();
  if (remove_standard_frame_ && window_pos->flags & SWP_FRAMECHANGED &&
      ui::win::IsAeroGlassEnabled() &&
      (window_ex_style() & WS_EX_COMPOSITED) == 0) {
    MARGINS m = {10, 10, 10, 10};
    DwmExtendFrameIntoClientArea(hwnd(), &m);
  }
  if (window_pos->flags & SWP_SHOWWINDOW)
    delegate_->HandleVisibilityChanged(true);
  else if (window_pos->flags & SWP_HIDEWINDOW)
    delegate_->HandleVisibilityChanged(false);
  SetMsgHandled(FALSE);
}
