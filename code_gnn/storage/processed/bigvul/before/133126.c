LRESULT HWNDMessageHandler::OnMouseRange(UINT message,
                                         WPARAM w_param,
                                         LPARAM l_param) {
#if defined(USE_AURA)
  if (!touch_ids_.empty())
    return 0;
  if (ui::IsMouseEventFromTouch(message)) {
    LPARAM l_param_ht = l_param;
    if (message != WM_MOUSEWHEEL && message != WM_MOUSEHWHEEL) {
      CPoint screen_point(l_param_ht);
      MapWindowPoints(hwnd(), HWND_DESKTOP, &screen_point, 1);
      l_param_ht = MAKELPARAM(screen_point.x, screen_point.y);
    }
    LRESULT hittest = SendMessage(hwnd(), WM_NCHITTEST, 0, l_param_ht);
    if (hittest == HTCLIENT || hittest == HTNOWHERE)
      return 0;
  }
#endif
  if (message == WM_RBUTTONUP && is_right_mouse_pressed_on_caption_) {
    is_right_mouse_pressed_on_caption_ = false;
    ReleaseCapture();
    CPoint screen_point(l_param);
    MapWindowPoints(hwnd(), HWND_DESKTOP, &screen_point, 1);
    w_param = SendMessage(hwnd(), WM_NCHITTEST, 0,
                          MAKELPARAM(screen_point.x, screen_point.y));
    if (w_param == HTCAPTION || w_param == HTSYSMENU) {
      gfx::ShowSystemMenuAtPoint(hwnd(), gfx::Point(screen_point));
      return 0;
    }
  } else if (message == WM_NCLBUTTONDOWN && delegate_->IsUsingCustomFrame()) {
    switch (w_param) {
      case HTCLOSE:
      case HTMINBUTTON:
      case HTMAXBUTTON: {
        w_param |= base::win::IsCtrlPressed() ? MK_CONTROL : 0;
        w_param |= base::win::IsShiftPressed() ? MK_SHIFT : 0;
      }
    }
  } else if (message == WM_NCRBUTTONDOWN &&
      (w_param == HTCAPTION || w_param == HTSYSMENU)) {
    is_right_mouse_pressed_on_caption_ = true;
    SetCapture();
  }

  MSG msg = { hwnd(), message, w_param, l_param, GetMessageTime(),
              { GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) } };
  ui::MouseEvent event(msg);
  if (!touch_ids_.empty() || ui::IsMouseEventFromTouch(message))
    event.set_flags(event.flags() | ui::EF_FROM_TOUCH);

  if (!(event.flags() & ui::EF_IS_NON_CLIENT))
    delegate_->HandleTooltipMouseMove(message, w_param, l_param);

  if (event.type() == ui::ET_MOUSE_MOVED && !HasCapture()) {
    TrackMouseEvents((message == WM_NCMOUSEMOVE) ?
        TME_NONCLIENT | TME_LEAVE : TME_LEAVE);
  } else if (event.type() == ui::ET_MOUSE_EXITED) {
    active_mouse_tracking_flags_ = 0;
  } else if (event.type() == ui::ET_MOUSEWHEEL) {
    return (ui::RerouteMouseWheel(hwnd(), w_param, l_param) ||
            delegate_->HandleMouseEvent(ui::MouseWheelEvent(msg))) ? 0 : 1;
  }

  base::WeakPtr<HWNDMessageHandler> ref(weak_factory_.GetWeakPtr());
  bool handled = delegate_->HandleMouseEvent(event);
  if (!ref.get())
    return 0;
  if (!handled && message == WM_NCLBUTTONDOWN && w_param != HTSYSMENU &&
      delegate_->IsUsingCustomFrame()) {
    DefWindowProcWithRedrawLock(message, w_param, l_param);
    handled = true;
  }

  if (ref.get())
    SetMsgHandled(handled);
  return 0;
}
