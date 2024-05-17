void WebContentsImpl::RequestToLockMouse(
    RenderWidgetHostImpl* render_widget_host,
    bool user_gesture,
    bool last_unlocked_by_target,
    bool privileged) {
  for (WebContentsImpl* current = this; current;
       current = current->GetOuterWebContents()) {
    if (current->mouse_lock_widget_) {
      render_widget_host->GotResponseToLockMouseRequest(false);
      return;
    }
  }

  if (privileged) {
    DCHECK(!GetOuterWebContents());
    mouse_lock_widget_ = render_widget_host;
    render_widget_host->GotResponseToLockMouseRequest(true);
    return;
  }

  bool widget_in_frame_tree = false;
  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    if (node->current_frame_host()->GetRenderWidgetHost() ==
        render_widget_host) {
      widget_in_frame_tree = true;
      break;
    }
  }

  if (widget_in_frame_tree && delegate_) {
    for (WebContentsImpl* current = this; current;
         current = current->GetOuterWebContents()) {
      current->mouse_lock_widget_ = render_widget_host;
    }

    delegate_->RequestToLockMouse(this, user_gesture, last_unlocked_by_target);
  } else {
    render_widget_host->GotResponseToLockMouseRequest(false);
  }
}
