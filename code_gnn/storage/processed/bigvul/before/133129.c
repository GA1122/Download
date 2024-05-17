LRESULT HWNDMessageHandler::OnNCActivate(UINT message,
                                         WPARAM w_param,
                                         LPARAM l_param) {
  BOOL active = static_cast<BOOL>(LOWORD(w_param));

  bool inactive_rendering_disabled = delegate_->IsInactiveRenderingDisabled();

  if (!delegate_->IsWidgetWindow()) {
    SetMsgHandled(FALSE);
    return 0;
  }

  if (!delegate_->CanActivate())
    return TRUE;

  if (active && inactive_rendering_disabled)
    delegate_->EnableInactiveRendering();

  if (delegate_->IsUsingCustomFrame()) {
    RedrawWindow(hwnd(), NULL, NULL,
                 RDW_NOCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW);
    EnumChildWindows(hwnd(), EnumChildWindowsForRedraw, NULL);
  }

  if (IsVisible())
    delegate_->SchedulePaint();

  if (delegate_->IsUsingCustomFrame() &&
      base::win::GetVersion() > base::win::VERSION_VISTA) {
    SetMsgHandled(TRUE);
    return TRUE;
  }

  return DefWindowProcWithRedrawLock(
      WM_NCACTIVATE, inactive_rendering_disabled || active, 0);
}
