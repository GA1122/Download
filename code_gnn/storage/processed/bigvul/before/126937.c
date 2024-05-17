void BrowserTabStripController::UpdateLayoutType() {
  bool adjust_layout = false;
  TabStripLayoutType layout_type =
      DetermineTabStripLayout(g_browser_process->local_state(), &adjust_layout);
  tabstrip_->SetLayoutType(layout_type, adjust_layout);
}
