void WebContentsImpl::OnFirstVisuallyNonEmptyPaint(RenderViewHostImpl* source) {
  for (auto& observer : observers_)
    observer.DidFirstVisuallyNonEmptyPaint();

  did_first_visually_non_empty_paint_ = true;

  if (theme_color_ != last_sent_theme_color_) {
    for (auto& observer : observers_)
      observer.DidChangeThemeColor(theme_color_);
    last_sent_theme_color_ = theme_color_;
  }
}
