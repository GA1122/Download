void RenderView::OnEnablePreferredSizeChangedMode(int flags) {
  DCHECK(flags != kPreferredSizeNothing);
  if (send_preferred_size_changes_)
    return;
  send_preferred_size_changes_ = true;

  if (webview())
    didUpdateLayout(webview()->mainFrame());
}
