SynchronousCompositorImpl::~SynchronousCompositorImpl() {
  SynchronousCompositorRegistryInProc::GetInstance()->UnregisterCompositor(
      routing_id_, this);
}
