void SynchronousCompositorImpl::SetMemoryPolicy(size_t bytes_limit) {
  DCHECK(CalledOnValidThread());
  DCHECK(output_surface_);

  size_t current_bytes_limit = output_surface_->GetMemoryPolicy();
  output_surface_->SetMemoryPolicy(bytes_limit);

  if (bytes_limit && !current_bytes_limit) {
    g_factory.Get().CompositorInitializedHardwareDraw();
  } else if (!bytes_limit && current_bytes_limit) {
    g_factory.Get().CompositorReleasedHardwareDraw();
  }
}
