SkPath GM2TabStyle::GetPath(PathType path_type,
                            float scale,
                            bool force_active,
                            RenderUnits render_units) const {
  const int stroke_thickness = GetStrokeThickness(force_active);

  gfx::RectF aligned_bounds =
      ScaleAndAlignBounds(tab_->bounds(), scale, stroke_thickness);

  if (path_type == PathType::kInteriorClip) {
    const auto opacities = GetSeparatorOpacities(true);
    constexpr float kChildClipPadding = 2.5f;
    aligned_bounds.Inset(gfx::InsetsF(0.0f, kChildClipPadding + opacities.left,
                                      0.0f,
                                      kChildClipPadding + opacities.right));
  }

  const float radius = GetTopCornerRadiusForWidth(tab_->width()) * scale;
  float top_radius = radius;
  float bottom_radius = radius;

  const float extension = GetCornerRadius() * scale;

  const float left = aligned_bounds.x();
  const float right = aligned_bounds.right();
  float tab_top = aligned_bounds.y();
  float tab_left = left + extension;
  float tab_right = right - extension;

  const float extended_bottom = aligned_bounds.bottom();
  const float bottom_extension =
      GetLayoutConstant(TABSTRIP_TOOLBAR_OVERLAP) * scale;
  float tab_bottom = extended_bottom - bottom_extension;

  const float stroke_adjustment = stroke_thickness * scale;
  if (path_type == PathType::kInteriorClip) {
    tab_left += stroke_adjustment;
    tab_right -= stroke_adjustment;
    tab_top += stroke_adjustment;
    top_radius -= stroke_adjustment;
  } else if (path_type == PathType::kFill || path_type == PathType::kBorder) {
    tab_left += 0.5f * stroke_adjustment;
    tab_right -= 0.5f * stroke_adjustment;
    tab_top += 0.5f * stroke_adjustment;
    top_radius -= 0.5f * stroke_adjustment;
    tab_bottom -= 0.5f * stroke_adjustment;
    bottom_radius -= 0.5f * stroke_adjustment;
  } else if (path_type == PathType::kHitTest ||
             path_type == PathType::kExteriorClip) {
    tab_bottom -= stroke_adjustment;
    bottom_radius -= stroke_adjustment;
  }
  const bool extend_to_top =
      (path_type == PathType::kHitTest) && ShouldExtendHitTest();

  const float corner_gap = (right - tab_right) - bottom_radius;

  SkPath path;

  if (path_type == PathType::kInteriorClip) {
    path.addRect(tab_left, tab_top, tab_right, tab_bottom);
  } else if (path_type == PathType::kHighlight) {
    const float inset = views::PlatformStyle::kFocusHaloThickness +
                        views::PlatformStyle::kFocusHaloInset;
    SkRRect rrect = SkRRect::MakeRectXY(
        SkRect::MakeLTRB(tab_left + inset, tab_top + inset, tab_right - inset,
                         tab_bottom - inset),
        radius - inset, radius - inset);
    path.addRRect(rrect);
  } else {


    path.moveTo(left, extended_bottom);
    path.lineTo(left, tab_bottom);
    path.lineTo(left + corner_gap, tab_bottom);

    path.arcTo(bottom_radius, bottom_radius, 0, SkPath::kSmall_ArcSize,
               SkPath::kCCW_Direction, tab_left, tab_bottom - bottom_radius);

    if (extend_to_top) {
      path.lineTo(tab_left, tab_top);
    } else {
      path.lineTo(tab_left, tab_top + top_radius);
      path.arcTo(top_radius, top_radius, 0, SkPath::kSmall_ArcSize,
                 SkPath::kCW_Direction, tab_left + top_radius, tab_top);
    }

    if (extend_to_top) {
      path.lineTo(tab_right, tab_top);

    } else {
      path.lineTo(tab_right - top_radius, tab_top);
      path.arcTo(top_radius, top_radius, 0, SkPath::kSmall_ArcSize,
                 SkPath::kCW_Direction, tab_right, tab_top + top_radius);
    }

    path.lineTo(tab_right, tab_bottom - bottom_radius);
    path.arcTo(bottom_radius, bottom_radius, 0, SkPath::kSmall_ArcSize,
               SkPath::kCCW_Direction, right - corner_gap, tab_bottom);

    path.lineTo(right, tab_bottom);
    path.lineTo(right, extended_bottom);

    if (path_type != PathType::kBorder)
      path.close();
  }

  gfx::PointF origin(tab_->origin());
  origin.Scale(scale);
  path.offset(-origin.x(), -origin.y());

  if (render_units == RenderUnits::kDips && scale != 1.0f)
    path.transform(SkMatrix::MakeScale(1.f / scale));

  return path;
}
