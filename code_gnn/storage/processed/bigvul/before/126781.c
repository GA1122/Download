gfx::Rect BrowserView::GetRootWindowResizerRect() const {
  if (frame_->IsMaximized() || frame_->IsFullscreen())
    return gfx::Rect();

  if (IsDownloadShelfVisible())
    return gfx::Rect();

  gfx::Rect client_rect = contents_split_->bounds();
  gfx::Size resize_corner_size = ResizeCorner::GetSize();
  int x = client_rect.width() - resize_corner_size.width();
  if (base::i18n::IsRTL())
    x = 0;
  return gfx::Rect(x, client_rect.height() - resize_corner_size.height(),
                   resize_corner_size.width(), resize_corner_size.height());
}
