RenderWidgetHostImpl::~RenderWidgetHostImpl() {
  SetView(NULL);

  BackingStoreManager::RemoveBackingStore(this);

  GpuSurfaceTracker::Get()->RemoveSurface(surface_id_);
  surface_id_ = 0;

  process_->Release(routing_id_);

  if (delegate_)
    delegate_->RenderWidgetDeleted(this);
}
