void WebContentsImpl::OnThemeColorChanged(RenderFrameHostImpl* source,
                                          SkColor theme_color) {
  if (source != GetMainFrame()) {
    return;
  }

  theme_color_ = theme_color;

  if (did_first_visually_non_empty_paint_ &&
      last_sent_theme_color_ != theme_color_) {
    for (auto& observer : observers_)
      observer.DidChangeThemeColor(theme_color_);
    last_sent_theme_color_ = theme_color_;
  }
}
