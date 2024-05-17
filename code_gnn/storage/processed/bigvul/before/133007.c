void RenderWidgetHostViewAura::ReleaseReferencesToSoftwareFrame() {
  ui::Compositor* compositor = GetCompositor();
  if (compositor) {
    AddOnCommitCallbackAndDisableLocks(base::Bind(
        &RenderWidgetHostViewAura::SendReclaimSoftwareFrames, AsWeakPtr()));
  }
  UpdateExternalTexture();
}
