void RenderWidgetHostViewAura::AcceleratedSurfaceRelease() {
  if (current_surface_.get()) {
    ui::Compositor* compositor = GetCompositor();
    if (compositor) {
      AddOnCommitCallbackAndDisableLocks(
          base::Bind(&RenderWidgetHostViewAura::SetSurfaceNotInUseByCompositor,
                     AsWeakPtr(),
                     current_surface_));   
    }
    current_surface_ = NULL;
    UpdateExternalTexture();
  }
}
