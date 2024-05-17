void LockScreenMediaControlsView::RunHideControlsAnimation() {
  ui::ScopedLayerAnimationSettings animation(
      contents_view_->layer()->GetAnimator());
  animation.AddObserver(this);
  animation.SetPreemptionStrategy(
      ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
  animation.SetTransitionDuration(kAnimationDuration);

  gfx::Transform transform;
  transform.Translate(0, -GetBoundsInScreen().bottom());
  contents_view_->layer()->SetTransform(transform);
  contents_view_->layer()->SetOpacity(0);
}
