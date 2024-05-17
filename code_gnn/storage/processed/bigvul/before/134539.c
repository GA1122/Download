void WebContentsViewAura::ResetOverscrollTransform() {
  if (!web_contents_->GetRenderWidgetHostView())
    return;
  aura::Window* target = GetWindowToAnimateForOverscroll();
  if (!target)
    return;
  {
    ui::ScopedLayerAnimationSettings settings(target->layer()->GetAnimator());
    settings.SetPreemptionStrategy(
        ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
    settings.SetTweenType(gfx::Tween::EASE_OUT);
    settings.AddObserver(this);
    target->SetTransform(gfx::Transform());
  }
  {
    ui::ScopedLayerAnimationSettings settings(target->layer()->GetAnimator());
    settings.SetPreemptionStrategy(
        ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
    settings.SetTweenType(gfx::Tween::EASE_OUT);
    UpdateOverscrollWindowBrightness(0.f);
  }
}
