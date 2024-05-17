aura::Window* WebContentsViewAura::GetWindowToAnimateForOverscroll() {
  if (current_overscroll_gesture_ == OVERSCROLL_NONE)
    return NULL;

  return ShouldNavigateForward(web_contents_->GetController(),
                               current_overscroll_gesture_) ?
      overscroll_window_.get() : GetContentNativeView();
}
