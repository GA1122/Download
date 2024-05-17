bool ForEachFrameInternal(
    const base::Callback<void(RenderFrameHost*)>& on_frame,
    FrameTreeNode* node) {
  on_frame.Run(node->current_frame_host());
  return true;
}
