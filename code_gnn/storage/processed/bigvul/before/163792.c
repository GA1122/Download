bool DelegatedFrameHost::TransformPointToCoordSpaceForView(
    const gfx::Point& point,
    RenderWidgetHostViewBase* target_view,
    gfx::Point* transformed_point) {
  if (!has_frame_)
    return false;

  return target_view->TransformPointToLocalCoordSpace(
      point, viz::SurfaceId(frame_sink_id_, local_surface_id_),
      transformed_point);
}
