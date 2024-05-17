void WebContentsViewAura::CompleteOverscrollNavigation(OverscrollMode mode) {
  if (!web_contents_->GetRenderWidgetHostView())
    return;

  if (current_overscroll_gesture_ == OVERSCROLL_NONE)
    return;

  UMA_HISTOGRAM_ENUMERATION("Overscroll.Navigated",
                            current_overscroll_gesture_, OVERSCROLL_COUNT);
  OverscrollWindowDelegate* delegate = static_cast<OverscrollWindowDelegate*>(
      overscroll_window_->delegate());
  delegate->stop_forwarding_events();

  completed_overscroll_gesture_ = mode;
  aura::Window* target = GetWindowToAnimateForOverscroll();
  ui::ScopedLayerAnimationSettings settings(target->layer()->GetAnimator());
  settings.SetPreemptionStrategy(
      ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
  settings.SetTweenType(gfx::Tween::EASE_OUT);
  settings.AddObserver(this);
  gfx::Transform transform;
  int content_width =
      web_contents_->GetRenderWidgetHostView()->GetViewBounds().width();
  int translate_x = mode == OVERSCROLL_WEST ? -content_width : content_width;
  transform.Translate(translate_x, 0);
  target->SetTransform(transform);
  UpdateOverscrollWindowBrightness(translate_x);
}
