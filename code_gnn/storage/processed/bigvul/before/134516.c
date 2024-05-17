void WebContentsViewAura::OnImplicitAnimationsCompleted() {
  overscroll_shadow_.reset();

  if (ShouldNavigateForward(web_contents_->GetController(),
                            completed_overscroll_gesture_)) {
    PrepareOverscrollNavigationOverlay();
    web_contents_->GetController().GoForward();
  } else if (ShouldNavigateBack(web_contents_->GetController(),
                                completed_overscroll_gesture_)) {
    PrepareOverscrollNavigationOverlay();
    web_contents_->GetController().GoBack();
  } else {
    if (touch_editable_)
      touch_editable_->OverscrollCompleted();
  }

  aura::Window* content = GetContentNativeView();
  if (content) {
    content->SetTransform(gfx::Transform());
    content->layer()->SetLayerBrightness(0.f);
  }
  current_overscroll_gesture_ = OVERSCROLL_NONE;
  completed_overscroll_gesture_ = OVERSCROLL_NONE;
  overscroll_window_.reset();
}
