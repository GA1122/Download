void Launcher::SetStatusSize(const gfx::Size& size) {
  if (status_size_ == size)
    return;

  status_size_ = size;
  delegate_view_->Layout();
}
