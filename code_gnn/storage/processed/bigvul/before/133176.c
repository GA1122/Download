void HWNDMessageHandler::ShowWindowWithState(ui::WindowShowState show_state) {
  TRACE_EVENT0("views", "HWNDMessageHandler::ShowWindowWithState");
  DWORD native_show_state;
  switch (show_state) {
    case ui::SHOW_STATE_INACTIVE:
      native_show_state = SW_SHOWNOACTIVATE;
      break;
    case ui::SHOW_STATE_MAXIMIZED:
      native_show_state = SW_SHOWMAXIMIZED;
      break;
    case ui::SHOW_STATE_MINIMIZED:
      native_show_state = SW_SHOWMINIMIZED;
      break;
    default:
      native_show_state = delegate_->GetInitialShowState();
      break;
  }
  Show(native_show_state);
}
