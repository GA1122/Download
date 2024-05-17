gfx::Size WebContentsImpl::GetSizeForNewRenderView() {
  gfx::Size size;
  if (delegate_)
    size = delegate_->GetSizeForNewRenderView(this);
  if (size.IsEmpty())
    size = GetContainerBounds().size();
  return size;
}
