RenderFrameHostImpl* WebContentsImpl::GetMainFrame() {
  return frame_tree_.root()->current_frame_host();
}
