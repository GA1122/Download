bool WebContentsImpl::HandleWheelEvent(
    const blink::WebMouseWheelEvent& event) {
#if !defined(OS_MACOSX)
  if (delegate_ && event.wheel_ticks_y &&
      !ui::WebInputEventTraits::CanCauseScroll(event)) {
    zoom_scroll_remainder_ += event.wheel_ticks_y;
    int whole_zoom_scroll_remainder_ = std::lround(zoom_scroll_remainder_);
    zoom_scroll_remainder_ -= whole_zoom_scroll_remainder_;
    if (whole_zoom_scroll_remainder_ != 0) {
      delegate_->ContentsZoomChange(whole_zoom_scroll_remainder_ > 0);
    }
    return true;
  }
#endif
  return false;
}
