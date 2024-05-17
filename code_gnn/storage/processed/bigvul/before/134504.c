gfx::Vector2d WebContentsViewAura::GetTranslationForOverscroll(int delta_x,
                                                               int delta_y) {
  if (current_overscroll_gesture_ == OVERSCROLL_NORTH ||
      current_overscroll_gesture_ == OVERSCROLL_SOUTH) {
    return gfx::Vector2d(0, delta_y);
  }
  const NavigationControllerImpl& controller = web_contents_->GetController();
  const gfx::Rect& bounds = GetViewBounds();
  if (ShouldNavigateForward(controller, current_overscroll_gesture_))
    return gfx::Vector2d(std::max(-bounds.width(), delta_x), 0);
  else if (ShouldNavigateBack(controller, current_overscroll_gesture_))
    return gfx::Vector2d(std::min(bounds.width(), delta_x), 0);
  return gfx::Vector2d();
}
