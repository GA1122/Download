 void OffscreenCanvasSurfaceImpl::Require(const cc::SurfaceId& surface_id,
                                          const cc::SurfaceSequence& sequence) {
   cc::SurfaceManager* manager = GetSurfaceManager();
  cc::Surface* surface = manager->GetSurfaceForId(surface_id);
  if (!surface) {
    DLOG(ERROR) << "Attempting to require callback on nonexistent surface";
    return;
  }
  surface->AddDestructionDependency(sequence);
}
