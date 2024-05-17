gfx::Size PageInfoBubbleView::CalculatePreferredSize() const {
  if (header_ == nullptr && site_settings_view_ == nullptr) {
    return views::View::CalculatePreferredSize();
  }

  int height = views::View::CalculatePreferredSize().height();
  int width = kMinBubbleWidth;
  if (site_settings_view_) {
    width = std::max(width, permissions_view_->GetPreferredSize().width());
  }
  width = std::min(width, kMaxBubbleWidth);
  return gfx::Size(width, height);
}
