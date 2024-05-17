bool SurfaceHitTestReadyNotifier::ContainsSurfaceId(
    const viz::SurfaceId& container_surface_id) {
  if (!container_surface_id.is_valid())
    return false;

  viz::Surface* container_surface =
      surface_manager_->GetSurfaceForId(container_surface_id);
  if (!container_surface || !container_surface->active_referenced_surfaces())
    return false;

  for (const viz::SurfaceId& id :
       *container_surface->active_referenced_surfaces()) {
    if (id == target_view_->GetCurrentSurfaceId() || ContainsSurfaceId(id))
      return true;
  }
  return false;
}
