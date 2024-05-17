void WebContentsViewAura::ShowContextMenu(const ContextMenuParams& params) {
  if (touch_editable_)
    touch_editable_->EndTouchEditing();
  if (delegate_) {
    delegate_->ShowContextMenu(params);
  }
}
