 RenderFrameHost* WebContentsImpl::GetFocusedFrameIncludingInnerWebContents() {
  WebContentsImpl* contents = this;
  FrameTreeNode* focused_node = contents->frame_tree_.GetFocusedFrame();

  if (!focused_node)
    return nullptr;

  while (true) {
    contents = contents->node_.GetInnerWebContentsInFrame(focused_node);
    if (!contents)
      return focused_node->current_frame_host();

    focused_node = contents->frame_tree_.GetFocusedFrame();
    if (!focused_node)
      return contents->GetMainFrame();
  }
}
