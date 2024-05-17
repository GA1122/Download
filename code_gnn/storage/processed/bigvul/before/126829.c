int BrowserView::NonClientHitTest(const gfx::Point& point) {
#if defined(OS_WIN) && !defined(USE_AURA)
  if (!frame_->IsMaximized() && !frame_->IsFullscreen()) {
    CRect client_rect;
    ::GetClientRect(frame_->GetNativeWindow(), &client_rect);
    gfx::Size resize_corner_size = ResizeCorner::GetSize();
    gfx::Rect resize_corner_rect(client_rect.right - resize_corner_size.width(),
        client_rect.bottom - resize_corner_size.height(),
        resize_corner_size.width(), resize_corner_size.height());
    bool rtl_dir = base::i18n::IsRTL();
    if (rtl_dir)
      resize_corner_rect.set_x(0);
    if (resize_corner_rect.Contains(point)) {
      if (rtl_dir)
        return HTBOTTOMLEFT;
      return HTBOTTOMRIGHT;
    }
  }
#endif

  return GetBrowserViewLayout()->NonClientHitTest(point);
}
