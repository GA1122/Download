bool WebContentsImpl::HandleWheelEvent(
    const blink::WebMouseWheelEvent& event) {
#if !defined(OS_MACOSX)
  if (delegate_ && event.wheelTicksY &&
      (event.modifiers & blink::WebInputEvent::ControlKey) &&
      !event.canScroll) {
    zoom_scroll_remainder_ += event.wheelTicksY;
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
