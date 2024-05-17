bool SynchronousCompositorOutputSurface::BindToClient(
    cc::OutputSurfaceClient* surface_client) {
  DCHECK(CalledOnValidThread());
  if (!cc::OutputSurface::BindToClient(surface_client))
    return false;

  client_->SetMemoryPolicy(memory_policy_);
  registry_->RegisterOutputSurface(routing_id_, this);
  registered_ = true;
  return true;
}
