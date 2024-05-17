void RenderWidgetHostViewAura::LockResources() {
  DCHECK(frame_provider_);
  delegated_frame_evictor_->LockFrame();
}
