void VoiceInteractionOverlay::EndAnimation() {
  if (IsBursting()) {
    return;
  }

  SkMScalar scale_factor =
      kRippleCircleStartRadiusDip / kRippleCircleInitRadiusDip;
  gfx::Transform transform;

  const gfx::Point center = host_view_->GetAppListButtonCenterPoint();
  transform.Translate(center.x() - kRippleCircleStartRadiusDip,
                      center.y() - kRippleCircleStartRadiusDip);
  transform.Scale(scale_factor, scale_factor);

  {
    ui::ScopedLayerAnimationSettings settings(ripple_layer_->GetAnimator());
    settings.SetPreemptionStrategy(ui::LayerAnimator::PreemptionStrategy::
                                       IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kFullRetractDurationMs));
    settings.SetTweenType(gfx::Tween::SLOW_OUT_LINEAR_IN);

    ripple_layer_->SetTransform(transform);

    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kRippleOpacityRetractDurationMs));
    ripple_layer_->SetOpacity(0);
  }

  if (!show_icon_)
    return;

  transform.MakeIdentity();

  transform.Translate(center.x() - kIconStartSizeDip / 2.f,
                      center.y() - kIconStartSizeDip / 2.f);

  scale_factor = kIconStartSizeDip / kIconInitSizeDip;
  transform.Scale(scale_factor, scale_factor);

  {
    ui::ScopedLayerAnimationSettings settings(icon_layer_->GetAnimator());
    settings.SetPreemptionStrategy(ui::LayerAnimator::PreemptionStrategy::
                                       IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kFullRetractDurationMs));
    settings.SetTweenType(gfx::Tween::SLOW_OUT_LINEAR_IN);

    icon_layer_->SetTransform(transform);
    icon_layer_->SetOpacity(0);
  }

  transform.MakeIdentity();

  transform.Translate(center.x() - kBackgroundStartSizeDip / 2.f,
                      center.y() - kBackgroundStartSizeDip / 2.f);

  scale_factor = kBackgroundStartSizeDip / kBackgroundInitSizeDip;
  transform.Scale(scale_factor, scale_factor);

  {
    ui::ScopedLayerAnimationSettings settings(background_layer_->GetAnimator());
    settings.SetPreemptionStrategy(ui::LayerAnimator::PreemptionStrategy::
                                       IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kFullRetractDurationMs));
    settings.SetTweenType(gfx::Tween::SLOW_OUT_LINEAR_IN);

    background_layer_->SetTransform(transform);
    background_layer_->SetOpacity(0);
  }
}
