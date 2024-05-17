void Compositor::SetLocalSurfaceId(
    const viz::LocalSurfaceId& local_surface_id) {
  host_->SetLocalSurfaceIdFromParent(local_surface_id);
}
