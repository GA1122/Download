void ChromeClientImpl::InvalidateRect(const IntRect& update_rect) {
  if (!update_rect.IsEmpty())
    web_view_->InvalidateRect(update_rect);
}
