gfx::Size WebContentsImpl::GetSizeForNewRenderView(bool is_main_frame) {
  gfx::Size size;
  if (is_main_frame)
    size = device_emulation_size_;
  if (size.IsEmpty() && delegate_)
    size = delegate_->GetSizeForNewRenderView(this);
  if (size.IsEmpty())
    size = GetContainerBounds().size();
  return size;
}
