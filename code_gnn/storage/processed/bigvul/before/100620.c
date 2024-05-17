void VoiceInteractionOverlay::WaitingAnimation() {
  if (IsBursting())
    return;

  animation_state_ = AnimationState::WAITING;

  gfx::Point center = host_view_->GetAppListButtonCenterPoint();
  gfx::Transform transform;

  ripple_layer_->SetOpacity(0);
  icon_layer_->SetOpacity(0);
  background_layer_->SetOpacity(0);
  SetVisible(true);

  {
    transform.Translate(kBackgroundLargeWidthDip / 2 + kBackgroundPaddingDip -
                            kIconEndSizeDip / 2,
                        -kBackgroundLargeHeightDip / 2 - kBackgroundPaddingDip -
                            kIconEndSizeDip / 2);
    SkMScalar scale_factor = kIconEndSizeDip / kIconInitSizeDip;
    transform.Scale(scale_factor, scale_factor);
    icon_layer_->SetTransform(transform);

    ui::ScopedLayerAnimationSettings settings(icon_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kBackgroundMorphDurationMs));
    settings.SetPreemptionStrategy(
        ui::LayerAnimator::PreemptionStrategy::ENQUEUE_NEW_ANIMATION);
    settings.SetTweenType(gfx::Tween::LINEAR_OUT_SLOW_IN);

    icon_layer_->SetOpacity(1);
    icon_layer_->StartAnimation();
  }

  {
    float x_offset = center.x() - kBackgroundSizeDip / 2;
    float y_offset = center.y() - kBackgroundSizeDip / 2;

    transform.MakeIdentity();
    background_layer_->SetTransform(transform);
    background_layer_->SetToLarge(gfx::PointF(
        kBackgroundLargeWidthDip / 2 + kBackgroundPaddingDip - x_offset,
        -kBackgroundLargeHeightDip / 2 - kBackgroundPaddingDip - y_offset));

    ui::ScopedLayerAnimationSettings settings(background_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kBackgroundMorphDurationMs));
    settings.SetPreemptionStrategy(
        ui::LayerAnimator::PreemptionStrategy::ENQUEUE_NEW_ANIMATION);
    settings.SetTweenType(gfx::Tween::LINEAR_OUT_SLOW_IN);

    background_layer_->SetOpacity(1);
  }
}
