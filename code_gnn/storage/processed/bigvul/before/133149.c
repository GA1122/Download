LRESULT HWNDMessageHandler::OnWndProc(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) {
  HWND window = hwnd();
  LRESULT result = 0;

  if (delegate_ && delegate_->PreHandleMSG(message, w_param, l_param, &result))
    return result;

#if !defined(USE_AURA)
  if (ProcessChildWindowMessage(message, w_param, l_param, &result))
    return result;
#endif

  const BOOL old_msg_handled = msg_handled_;
  base::WeakPtr<HWNDMessageHandler> ref(weak_factory_.GetWeakPtr());
  const BOOL processed =
      _ProcessWindowMessage(window, message, w_param, l_param, result, 0);
  if (!ref)
    return 0;
  msg_handled_ = old_msg_handled;

  if (!processed)
    result = DefWindowProc(window, message, w_param, l_param);

  if (!::IsWindow(window))
    return result;

  if (delegate_)
    delegate_->PostHandleMSG(message, w_param, l_param);
  if (message == WM_NCDESTROY) {
#if !defined(USE_AURA)
    base::MessageLoopForUI::current()->RemoveObserver(this);
#endif
    if (delegate_)
      delegate_->HandleDestroyed();
  }

  if (message == WM_ACTIVATE && delegate_->CanSaveFocus())
    PostProcessActivateMessage(LOWORD(w_param), !!HIWORD(w_param));

  if (message == WM_ENABLE && restore_focus_when_enabled_) {
    DCHECK(delegate_->CanSaveFocus());
    restore_focus_when_enabled_ = false;
    delegate_->RestoreFocusOnEnable();
  }
  return result;
}
