void BrowserTabStripController::LayoutTypeMaybeChanged() {
  bool adjust_layout = false;
  TabStripLayoutType layout_type =
      DetermineTabStripLayout(g_browser_process->local_state(), &adjust_layout);
  if (!adjust_layout || layout_type == tabstrip_->layout_type())
    return;

  g_browser_process->local_state()->SetInteger(
      prefs::kTabStripLayoutType,
      static_cast<int>(tabstrip_->layout_type()));
}
