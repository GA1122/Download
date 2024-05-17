RenderFrameHostImpl* WebContentsImpl::GetMainFrame() const {
  return frame_tree_.root()->current_frame_host();
}
