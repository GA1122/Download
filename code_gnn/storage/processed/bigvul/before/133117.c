void HWNDMessageHandler::OnExitMenuLoop(BOOL is_shortcut_menu) {
  if (--menu_depth_ == 0)
    delegate_->HandleMenuLoop(false);
  DCHECK_GE(0, menu_depth_);
}
