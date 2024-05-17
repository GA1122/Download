void LayerTreeHost::SetPrioritiesForSurfaces(size_t surface_memory_bytes) {
  DCHECK(surface_memory_placeholder_);

  surface_memory_placeholder_->SetTextureManager(
      contents_texture_manager_.get());
  surface_memory_placeholder_->set_request_priority(
      PriorityCalculator::RenderSurfacePriority());
  surface_memory_placeholder_->SetToSelfManagedMemoryPlaceholder(
      surface_memory_bytes);
}
