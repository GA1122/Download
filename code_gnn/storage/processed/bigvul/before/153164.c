gfx::AcceleratedWidget Compositor::ReleaseAcceleratedWidget() {
  DCHECK(!IsVisible());
  host_->ReleaseLayerTreeFrameSink();
  context_factory_->RemoveCompositor(this);
  context_creation_weak_ptr_factory_.InvalidateWeakPtrs();
  widget_valid_ = false;
  gfx::AcceleratedWidget widget = widget_;
  widget_ = gfx::kNullAcceleratedWidget;
  return widget;
}
