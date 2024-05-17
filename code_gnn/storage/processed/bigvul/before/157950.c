void RenderViewImpl::OnEnablePreferredSizeChangedMode() {
  if (send_preferred_size_changes_)
    return;
  send_preferred_size_changes_ = true;

  if (!webview())
    return;

  needs_preferred_size_update_ = true;

  webview()->UpdateLifecycle(WebWidget::LifecycleUpdate::kLayout);

  UpdatePreferredSize();
}
