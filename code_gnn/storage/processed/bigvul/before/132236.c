void RenderFrameImpl::OnDisownOpener() {
  CHECK(!frame_->parent());

  if (frame_->opener())
    frame_->setOpener(NULL);
}
