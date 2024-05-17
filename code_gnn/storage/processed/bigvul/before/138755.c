void RenderFrameHostImpl::OnFrameFocused() {
  delegate_->SetFocusedFrame(frame_tree_node_, GetSiteInstance());
}
