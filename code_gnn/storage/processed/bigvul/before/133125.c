LRESULT HWNDMessageHandler::OnMouseActivate(UINT message,
                                            WPARAM w_param,
                                            LPARAM l_param) {
#if defined(USE_AURA)
  if (::GetProp(hwnd(), kIgnoreTouchMouseActivateForWindow)) {
    ::RemoveProp(hwnd(), kIgnoreTouchMouseActivateForWindow);
    return MA_NOACTIVATE;
  }
  POINT cursor_pos = {0};
  ::GetCursorPos(&cursor_pos);
  ::ScreenToClient(hwnd(), &cursor_pos);
  HWND child = ::RealChildWindowFromPoint(hwnd(), cursor_pos);
  if (::IsWindow(child) && child != hwnd() && ::IsWindowVisible(child))
    PostProcessActivateMessage(WA_INACTIVE, false);
#endif
  if (delegate_->IsWidgetWindow())
    return delegate_->CanActivate() ? MA_ACTIVATE : MA_NOACTIVATEANDEAT;
  if (GetWindowLong(hwnd(), GWL_EXSTYLE) & WS_EX_NOACTIVATE)
    return MA_NOACTIVATE;
  SetMsgHandled(FALSE);
  return MA_ACTIVATE;
}
