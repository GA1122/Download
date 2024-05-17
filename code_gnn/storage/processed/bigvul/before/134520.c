void WebContentsViewAura::OnOverscrollModeChange(OverscrollMode old_mode,
                                                 OverscrollMode new_mode) {
  ResetOverscrollTransform();

  if (new_mode != OVERSCROLL_NONE && touch_editable_)
    touch_editable_->OverscrollStarted();

  if (new_mode == OVERSCROLL_NONE ||
      !GetContentNativeView() ||
      ((new_mode == OVERSCROLL_EAST || new_mode == OVERSCROLL_WEST) &&
       navigation_overlay_.get() && navigation_overlay_->has_window())) {
    current_overscroll_gesture_ = OVERSCROLL_NONE;
    OverscrollUpdateForWebContentsDelegate(0);
  } else {
    aura::Window* target = GetWindowToAnimateForOverscroll();
    if (target) {
      StopObservingImplicitAnimations();
      target->layer()->GetAnimator()->AbortAllAnimations();
    }
    PrepareContentWindowForOverscroll();

    current_overscroll_gesture_ = new_mode;
    if (current_overscroll_gesture_ == OVERSCROLL_EAST ||
        current_overscroll_gesture_ == OVERSCROLL_WEST)
      PrepareOverscrollWindow();

    UMA_HISTOGRAM_ENUMERATION("Overscroll.Started", new_mode, OVERSCROLL_COUNT);
  }
  completed_overscroll_gesture_ = OVERSCROLL_NONE;
}
