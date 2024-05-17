void VoiceInteractionOverlay::BurstAnimation() {
  animation_state_ = AnimationState::BURSTING;

  gfx::Point center = host_view_->GetAppListButtonCenterPoint();
  gfx::Transform transform;

  {
    SkMScalar scale_factor =
        kRippleCircleBurstRadiusDip / kRippleCircleInitRadiusDip;
    transform.Translate(center.x() - kRippleCircleBurstRadiusDip,
                        center.y() - kRippleCircleBurstRadiusDip);
    transform.Scale(scale_factor, scale_factor);

    ui::ScopedLayerAnimationSettings settings(ripple_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kFullBurstDurationMs));
    settings.SetTweenType(gfx::Tween::LINEAR_OUT_SLOW_IN);
    settings.SetPreemptionStrategy(
        ui::LayerAnimator::PreemptionStrategy::ENQUEUE_NEW_ANIMATION);

    ripple_layer_->SetTransform(transform);
    ripple_layer_->SetOpacity(0);
  }

  if (!show_icon_)
    return;

  {
    ui::ScopedLayerAnimationSettings settings(icon_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kBackgroundMorphDurationMs));
    settings.SetPreemptionStrategy(
        ui::LayerAnimator::PreemptionStrategy::ENQUEUE_NEW_ANIMATION);
    settings.SetTweenType(gfx::Tween::LINEAR_OUT_SLOW_IN);

    transform.MakeIdentity();
    transform.Translate(kBackgroundLargeWidthDip / 2 + kBackgroundPaddingDip -
                            kIconEndSizeDip / 2,
                        -kBackgroundLargeHeightDip / 2 - kBackgroundPaddingDip -
                            kIconEndSizeDip / 2);
    SkMScalar scale_factor = kIconEndSizeDip / kIconInitSizeDip;
    transform.Scale(scale_factor, scale_factor);

    icon_layer_->SetTransform(transform);
    icon_layer_->StartAnimation();
  }

  const bool is_tablet_mode = Shell::Get()
                                  ->tablet_mode_controller()
                                  ->IsTabletModeWindowManagerEnabled();
  const int icon_x_offset = is_tablet_mode ? 0 : kIconOffsetDip;
  const int icon_y_offset =
      is_tablet_mode ? -kRippleCircleRadiusDip : -kIconOffsetDip;
  float x_offset = center.x() - kBackgroundSizeDip / 2 + icon_x_offset;
  float y_offset = center.y() - kBackgroundSizeDip / 2 + icon_y_offset;

  background_layer_->AnimateToLarge(
      gfx::PointF(
          kBackgroundLargeWidthDip / 2 + kBackgroundPaddingDip - x_offset,
          -kBackgroundLargeHeightDip / 2 - kBackgroundPaddingDip - y_offset),
      nullptr);
}
