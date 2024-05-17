void ScrollableShelfView::StartShelfScrollAnimation(float scroll_distance) {
  const gfx::Transform current_transform = shelf_view_->GetTransform();
  gfx::Transform reverse_transform = current_transform;
  if (ShouldAdaptToRTL())
    scroll_distance = -scroll_distance;
  if (GetShelf()->IsHorizontalAlignment())
    reverse_transform.Translate(gfx::Vector2dF(scroll_distance, 0));
  else
    reverse_transform.Translate(gfx::Vector2dF(0, scroll_distance));
  shelf_view_->layer()->SetTransform(reverse_transform);
  ui::ScopedLayerAnimationSettings animation_settings(
      shelf_view_->layer()->GetAnimator());
  animation_settings.SetTweenType(gfx::Tween::EASE_OUT);
  animation_settings.SetPreemptionStrategy(
      ui::LayerAnimator::IMMEDIATELY_SET_NEW_TARGET);
  shelf_view_->layer()->SetTransform(current_transform);
}
