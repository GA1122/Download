bool RenderWidgetHostViewAura::TransformPointToCoordSpaceForView(
    const gfx::Point& point,
    RenderWidgetHostViewBase* target_view,
    gfx::Point* transformed_point) {
  if (target_view == this || !delegated_frame_host_) {
    *transformed_point = point;
    return true;
  }

  return delegated_frame_host_->TransformPointToCoordSpaceForView(
      point, target_view, transformed_point);
}
