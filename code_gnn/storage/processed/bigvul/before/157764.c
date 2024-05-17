void WebContentsImpl::ForEachFrame(
    const base::RepeatingCallback<void(RenderFrameHost*)>& on_frame) {
  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    on_frame.Run(node->current_frame_host());
  }
}
