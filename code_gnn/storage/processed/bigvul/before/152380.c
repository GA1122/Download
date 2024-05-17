void RenderFrameImpl::InitializeBlameContext(RenderFrameImpl* parent_frame) {
  DCHECK(!blame_context_);
  blame_context_ = std::make_unique<FrameBlameContext>(this, parent_frame);
  blame_context_->Initialize();
}
