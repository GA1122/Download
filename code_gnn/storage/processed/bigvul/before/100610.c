  void MoveLargeShadow(const gfx::PointF& new_center) {
    gfx::Transform transform;
    transform.Translate(new_center.x() - kBackgroundLargeWidthDip / 2,
                        new_center.y() - kBackgroundLargeHeightDip / 2);
    large_shadow_layer_->SetTransform(transform);
  }
