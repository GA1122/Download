    ~SynchronousCompositorExternalBeginFrameSource() {
  DCHECK(CalledOnValidThread());

  if (registered_) {
    SynchronousCompositorRegistry::GetInstance()->UnregisterBeginFrameSource(
        routing_id_, this);
  }
  DCHECK(!compositor_);
}
