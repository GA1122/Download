void RenderViewImpl::ForceResizeForTesting(const gfx::Size& new_size) {
  gfx::Rect new_window_rect(RootWindowRect().x, RootWindowRect().y,
                            new_size.width(), new_size.height());
  SetWindowRectSynchronously(new_window_rect);
}
