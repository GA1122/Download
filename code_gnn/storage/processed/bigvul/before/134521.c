void WebContentsViewAura::OnOverscrollUpdate(float delta_x, float delta_y) {
  if (current_overscroll_gesture_ == OVERSCROLL_NONE)
    return;

  aura::Window* target = GetWindowToAnimateForOverscroll();
  ui::ScopedLayerAnimationSettings settings(target->layer()->GetAnimator());
  settings.SetPreemptionStrategy(ui::LayerAnimator::IMMEDIATELY_SET_NEW_TARGET);
  gfx::Vector2d translate = GetTranslationForOverscroll(delta_x, delta_y);
  gfx::Transform transform;

  if (current_overscroll_gesture_ != OVERSCROLL_NORTH &&
      current_overscroll_gesture_ != OVERSCROLL_SOUTH) {
    transform.Translate(translate.x(), translate.y());
    target->SetTransform(transform);
    UpdateOverscrollWindowBrightness(delta_x);
  }

  OverscrollUpdateForWebContentsDelegate(translate.y());
}
