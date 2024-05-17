  gfx::Transform CalculateRectTransform(float x_scale, float y_scale) const {
    gfx::Transform transform;
    transform.Translate(center_point_.x(), center_point_.y());
    transform.Scale(x_scale, y_scale);
    const gfx::Vector2dF rect_center_offset =
        rect_layer_delegate_->GetCenteringOffset();
    transform.Translate(-rect_center_offset.x(), -rect_center_offset.y());
    return transform;
  }
