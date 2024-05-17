  gfx::Transform CalculateCircleTransform(float scale,
                                          float target_center_x,
                                          float target_center_y) const {
    gfx::Transform transform;
    transform.Translate(center_point_.x(), center_point_.y());
    transform.Translate(target_center_x, target_center_y);
    transform.Scale(scale, scale);
    const gfx::Vector2dF circle_center_offset =
        circle_layer_delegate_->GetCenteringOffset();
    transform.Translate(-circle_center_offset.x(), -circle_center_offset.y());
    return transform;
  }
