void RenderWidgetHostViewAura::SnapToPhysicalPixelBoundary() {
#if defined(OS_CHROMEOS)
  aura::Window* snapped = window_->GetToplevelWindow();
#else
  aura::Window* snapped = window_->GetRootWindow();
#endif

  if (snapped && snapped != window_)
    ui::SnapLayerToPhysicalPixelBoundary(snapped->layer(), window_->layer());

  has_snapped_to_boundary_ = true;
}
