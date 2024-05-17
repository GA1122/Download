void RenderWidgetHostViewAura::FatalAccessibilityTreeError() {
  host_->FatalAccessibilityTreeError();
  SetBrowserAccessibilityManager(NULL);
}
