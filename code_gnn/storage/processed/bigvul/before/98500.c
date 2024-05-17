void AeroPeekWindow::Update(bool is_loading) {
  if (!hwnd()) {
    gfx::Rect bounds;
    WindowImpl::Init(frame_window_, bounds);
    return;
  }

  if (tab_active_ || !is_loading)
    UpdateThumbnail();
}
