void GpuProcessHost::OnDestroyCommandBuffer(int32 surface_id) {
#if defined(TOOLKIT_GTK)
  SurfaceRefMap::iterator it = surface_refs_.find(surface_id);
  if (it != surface_refs_.end())
    surface_refs_.erase(it);
#endif   
}
