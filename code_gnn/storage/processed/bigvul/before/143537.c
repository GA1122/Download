void CompositorImpl::SetWindowBounds(const gfx::Size& size) {
  if (size_ == size)
    return;

  size_ = size;
  if (host_) {
    host_->SetViewportSizeAndScale(size_, root_window_->GetDipScale(),
                                   GenerateLocalSurfaceId());
  }
  if (display_)
    display_->Resize(size);

  if (display_private_)
    display_private_->Resize(size);

  root_window_->GetLayer()->SetBounds(size);
}
