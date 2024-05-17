void VoiceInteractionOverlay::StartAnimation(bool show_icon) {
  animation_state_ = AnimationState::STARTING;
  show_icon_ = show_icon;
  SetVisible(true);

  ripple_layer_->SetOpacity(0);

  SkMScalar scale_factor =
      kRippleCircleStartRadiusDip / kRippleCircleInitRadiusDip;
  gfx::Transform transform;

  const gfx::Point center = host_view_->GetAppListButtonCenterPoint();
  transform.Translate(center.x() - kRippleCircleStartRadiusDip,
                      center.y() - kRippleCircleStartRadiusDip);
  transform.Scale(scale_factor, scale_factor);
  ripple_layer_->SetTransform(transform);

  {
    scale_factor = kRippleCircleRadiusDip / kRippleCircleInitRadiusDip;
    transform.MakeIdentity();
    transform.Translate(center.x() - kRippleCircleRadiusDip,
                        center.y() - kRippleCircleRadiusDip);
    transform.Scale(scale_factor, scale_factor);

    ui::ScopedLayerAnimationSettings settings(ripple_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kRippleExpandDurationMs));
    settings.SetTweenType(gfx::Tween::FAST_OUT_SLOW_IN_2);

    ripple_layer_->SetTransform(transform);

    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kRippleOpacityDurationMs));
    ripple_layer_->SetOpacity(kRippleOpacity);
  }

  icon_layer_->SetOpacity(0);
  background_layer_->SetOpacity(0);
  if (!show_icon_)
    return;

  transform.MakeIdentity();
  transform.Translate(center.x() - kIconStartSizeDip / 2.f,
                      center.y() - kIconStartSizeDip / 2.f);

  scale_factor = kIconStartSizeDip / kIconInitSizeDip;
  transform.Scale(scale_factor, scale_factor);
  icon_layer_->SetTransform(transform);

  const bool is_tablet_mode = Shell::Get()
                                  ->tablet_mode_controller()
                                  ->IsTabletModeWindowManagerEnabled();
  const int icon_x_offset = is_tablet_mode ? 0 : kIconOffsetDip;
  const int icon_y_offset =
      is_tablet_mode ? -kRippleCircleRadiusDip : -kIconOffsetDip;
  scale_factor = kIconSizeDip / kIconInitSizeDip;
  transform.MakeIdentity();
  transform.Translate(center.x() - kIconSizeDip / 2 + icon_x_offset,
                      center.y() - kIconSizeDip / 2 + icon_y_offset);
  transform.Scale(scale_factor, scale_factor);

  {
    ui::ScopedLayerAnimationSettings settings(icon_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kFullExpandDurationMs));
    settings.SetTweenType(gfx::Tween::FAST_OUT_SLOW_IN_2);

    icon_layer_->SetTransform(transform);
    icon_layer_->SetOpacity(kIconOpacity);
  }

  background_layer_->ResetShape();

  transform.MakeIdentity();
  transform.Translate(center.x() - kBackgroundStartSizeDip / 2.f,
                      center.y() - kBackgroundStartSizeDip / 2.f);

  scale_factor = kBackgroundStartSizeDip / kBackgroundInitSizeDip;
  transform.Scale(scale_factor, scale_factor);
  background_layer_->SetTransform(transform);

  scale_factor = kBackgroundSizeDip / kBackgroundInitSizeDip;
  transform.MakeIdentity();
  transform.Translate(center.x() - kBackgroundSizeDip / 2 + icon_x_offset,
                      center.y() - kBackgroundSizeDip / 2 + icon_y_offset);
  transform.Scale(scale_factor, scale_factor);

  {
    ui::ScopedLayerAnimationSettings settings(background_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kFullExpandDurationMs));
    settings.SetTweenType(gfx::Tween::FAST_OUT_SLOW_IN_2);

    background_layer_->SetTransform(transform);
  }

  {
    ui::ScopedLayerAnimationSettings settings(background_layer_->GetAnimator());
    settings.SetTransitionDuration(
        base::TimeDelta::FromMilliseconds(kBackgroundOpacityDurationMs));
    settings.SetTweenType(gfx::Tween::FAST_OUT_SLOW_IN_2);

    background_layer_->SetOpacity(1);
  }
}
