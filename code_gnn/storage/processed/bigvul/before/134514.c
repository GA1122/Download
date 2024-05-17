void WebContentsViewAura::OnDragExited() {
  DCHECK(current_rvh_for_drag_);
  if (current_rvh_for_drag_ != web_contents_->GetRenderViewHost())
    return;

  web_contents_->GetRenderViewHost()->DragTargetDragLeave();
  if (drag_dest_delegate_)
    drag_dest_delegate_->OnDragLeave();

  current_drop_data_.reset();
}
