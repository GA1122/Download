void WebContentsImpl::OnThemeColorChanged(SkColor theme_color) {
  theme_color_ = theme_color;

  if (did_first_visually_non_empty_paint_ &&
      last_sent_theme_color_ != theme_color_) {
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      DidChangeThemeColor(theme_color_));
    last_sent_theme_color_ = theme_color_;
  }
}
