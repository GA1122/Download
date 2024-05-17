void BrowserCompositorOutputSurfaceProxy::OnUpdateVSyncParameters(
    int surface_id, base::TimeTicks timebase, base::TimeDelta interval) {
  BrowserCompositorOutputSurface* surface = surface_map_.Lookup(surface_id);
  if (surface)
    surface->OnUpdateVSyncParameters(timebase, interval);
}
