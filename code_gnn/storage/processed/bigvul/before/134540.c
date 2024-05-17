void WebContentsViewAura::RestoreFocus() {
  if (delegate_)
    delegate_->RestoreFocus();
}
