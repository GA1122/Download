GpuProcessHost::SurfaceRef::SurfaceRef(gfx::PluginWindowHandle surface)
    : surface_(surface) {
  GtkNativeViewManager* manager = GtkNativeViewManager::GetInstance();
  if (!manager->AddRefPermanentXID(surface_)) {
    LOG(ERROR) << "Surface " << surface << " cannot be referenced.";
  }
}
