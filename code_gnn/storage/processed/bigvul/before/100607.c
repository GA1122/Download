  void CalculateRectTransforms(const gfx::Size& desired_size,
                               float corner_radius,
                               PaintedShapeTransforms* transforms_out) const {
    DCHECK_GE(desired_size.width() / 2.0f, corner_radius)
        << "The circle's diameter should not be greater than the total width.";
    DCHECK_GE(desired_size.height() / 2.0f, corner_radius)
        << "The circle's diameter should not be greater than the total height.";

    gfx::SizeF size(desired_size);
    if (GetCompositor()) {
      const float dsf = GetCompositor()->device_scale_factor();
      gfx::RectF ripple_bounds((gfx::PointF(center_point_)), gfx::SizeF());
      ripple_bounds.Inset(-gfx::InsetsF(desired_size.height() / 2.0f,
                                        desired_size.width() / 2.0f));
      ripple_bounds.Scale(dsf);
      ripple_bounds = gfx::RectF(gfx::ToEnclosingRect(ripple_bounds));
      ripple_bounds.Scale(1.0f / dsf);
      size = ripple_bounds.size();
    }


    const float circle_scale = std::max(
        kMinimumCircleScale,
        corner_radius / static_cast<float>(circle_layer_delegate_->radius()));

    const float circle_target_x_offset = size.width() / 2.0f - corner_radius;
    const float circle_target_y_offset = size.height() / 2.0f - corner_radius;

    (*transforms_out)[TOP_LEFT_CIRCLE] = CalculateCircleTransform(
        circle_scale, -circle_target_x_offset, -circle_target_y_offset);
    (*transforms_out)[TOP_RIGHT_CIRCLE] = CalculateCircleTransform(
        circle_scale, circle_target_x_offset, -circle_target_y_offset);
    (*transforms_out)[BOTTOM_RIGHT_CIRCLE] = CalculateCircleTransform(
        circle_scale, circle_target_x_offset, circle_target_y_offset);
    (*transforms_out)[BOTTOM_LEFT_CIRCLE] = CalculateCircleTransform(
        circle_scale, -circle_target_x_offset, circle_target_y_offset);

    const float rect_delegate_width = rect_layer_delegate_->size().width();
    const float rect_delegate_height = rect_layer_delegate_->size().height();

    (*transforms_out)[HORIZONTAL_RECT] = CalculateRectTransform(
        std::max(kMinimumRectScale, size.width() / rect_delegate_width),
        std::max(kMinimumRectScale, (size.height() - 2.0f * corner_radius) /
                                        rect_delegate_height));

    (*transforms_out)[VERTICAL_RECT] = CalculateRectTransform(
        std::max(kMinimumRectScale,
                 (size.width() - 2.0f * corner_radius) / rect_delegate_width),
        std::max(kMinimumRectScale, size.height() / rect_delegate_height));
  }
