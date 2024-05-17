void WebContentsViewAura::StoreFocus() {
  if (delegate_)
    delegate_->StoreFocus();
}
