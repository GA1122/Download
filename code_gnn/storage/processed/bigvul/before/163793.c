bool DelegatedFrameHost::TransformPointToLocalCoordSpace(
    const gfx::Point& point,
    const viz::SurfaceId& original_surface,
    gfx::Point* transformed_point) {
  viz::SurfaceId surface_id(frame_sink_id_, local_surface_id_);
  if (!surface_id.is_valid())
    return false;
  *transformed_point = point;
  if (original_surface == surface_id)
    return true;

  viz::SurfaceHittest hittest(nullptr,
                              GetFrameSinkManager()->surface_manager());
  return hittest.TransformPointToTargetSurface(original_surface, surface_id,
                                               transformed_point);
}
