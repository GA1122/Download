void SynchronousCompositorOutputSurface::DetachFromClient() {
  DCHECK(CalledOnValidThread());
  if (registered_) {
    registry_->UnregisterOutputSurface(routing_id_, this);
  }
  cc::OutputSurface::DetachFromClient();
}
