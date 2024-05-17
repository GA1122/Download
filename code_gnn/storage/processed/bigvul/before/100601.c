  void AnimateToLarge(const gfx::PointF& new_center,
                      ui::LayerAnimationObserver* animation_observer) {
    PaintedShapeTransforms transforms;
    CalculateCircleTransforms(small_size_, &transforms);
    SetTransforms(transforms);
    SetPaintedLayersVisible(true);

    shadow_layer_->SetVisible(false);
    large_shadow_layer_->SetVisible(false);
    MoveLargeShadow(new_center);

    center_point_ = new_center;
    CalculateRectTransforms(large_size_, kBackgroundCornerRadiusDip,
                            &transforms);

    AnimateToTransforms(
        transforms,
        base::TimeDelta::FromMilliseconds(kBackgroundMorphDurationMs),
        ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET,
        gfx::Tween::LINEAR_OUT_SLOW_IN, animation_observer);
  }
