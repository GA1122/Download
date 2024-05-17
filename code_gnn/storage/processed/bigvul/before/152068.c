void RenderFrameHostImpl::ResetChildren() {
  std::vector<std::unique_ptr<FrameTreeNode>> children;
  children.swap(children_);
  for (auto& child : children)
    child->current_frame_host()->DeleteRenderFrame();
}
