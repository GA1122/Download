void RenderFrameHostImpl::OnFrameFocused() {
  if (!is_active())
    return;

  delegate_->SetFocusedFrame(frame_tree_node_, GetSiteInstance());
}
