void HTMLCanvasElement::SetSurfaceSize(const IntSize& size) {
  size_ = size;
  did_fail_to_create_resource_provider_ = false;
  DiscardResourceProvider();
  if (Is2d() && context_->isContextLost())
    context_->DidSetSurfaceSize();
  if (frame_dispatcher_)
    frame_dispatcher_->Reshape(size_);
}
