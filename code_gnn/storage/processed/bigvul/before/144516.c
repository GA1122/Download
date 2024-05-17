gfx::Rect WebContentsImpl::GetContainerBounds() {
  gfx::Rect rv;
  view_->GetContainerBounds(&rv);
  return rv;
}
