gfx::Vector2dF LayerTreeHostImpl::ScrollNodeWithViewportSpaceDelta(
    ScrollNode* scroll_node,
    const gfx::PointF& viewport_point,
    const gfx::Vector2dF& viewport_delta,
    ScrollTree* scroll_tree) {
  const gfx::Transform screen_space_transform =
      scroll_tree->ScreenSpaceTransform(scroll_node->id);
  DCHECK(screen_space_transform.IsInvertible());
  gfx::Transform inverse_screen_space_transform(
      gfx::Transform::kSkipInitialization);
  bool did_invert =
      screen_space_transform.GetInverse(&inverse_screen_space_transform);
  DCHECK(did_invert);

  float scale_from_viewport_to_screen_space =
      active_tree_->device_scale_factor();
  gfx::PointF screen_space_point =
      gfx::ScalePoint(viewport_point, scale_from_viewport_to_screen_space);

  gfx::Vector2dF screen_space_delta = viewport_delta;
  screen_space_delta.Scale(scale_from_viewport_to_screen_space);

  bool start_clipped, end_clipped;
  gfx::PointF screen_space_end_point = screen_space_point + screen_space_delta;
  gfx::PointF local_start_point = MathUtil::ProjectPoint(
      inverse_screen_space_transform, screen_space_point, &start_clipped);
  gfx::PointF local_end_point = MathUtil::ProjectPoint(
      inverse_screen_space_transform, screen_space_end_point, &end_clipped);

  DCHECK(!start_clipped);
  DCHECK(!end_clipped);
  if (start_clipped || end_clipped)
    return gfx::Vector2dF();

  gfx::ScrollOffset previous_offset =
      scroll_tree->current_scroll_offset(scroll_node->owning_layer_id);
  scroll_tree->ScrollBy(scroll_node, local_end_point - local_start_point,
                        active_tree());
  gfx::ScrollOffset scrolled =
      scroll_tree->current_scroll_offset(scroll_node->owning_layer_id) -
      previous_offset;

  gfx::PointF actual_local_end_point =
      local_start_point + gfx::Vector2dF(scrolled.x(), scrolled.y());

  gfx::PointF actual_screen_space_end_point = MathUtil::MapPoint(
      screen_space_transform, actual_local_end_point, &end_clipped);
  DCHECK(!end_clipped);
  if (end_clipped)
    return gfx::Vector2dF();
  gfx::PointF actual_viewport_end_point = gfx::ScalePoint(
      actual_screen_space_end_point, 1.f / scale_from_viewport_to_screen_space);
  return actual_viewport_end_point - viewport_point;
}
