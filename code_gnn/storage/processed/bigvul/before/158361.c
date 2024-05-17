void RenderWidgetHostImpl::SetFrameDepth(unsigned int depth) {
  if (frame_depth_ == depth)
    return;

  frame_depth_ = depth;
  UpdatePriority();
}
