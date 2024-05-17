void RenderWidgetHostViewAura::HandleParentBoundsChanged() {
  SnapToPhysicalPixelBoundary();
#if defined(OS_WIN)
  if (legacy_render_widget_host_HWND_) {
    legacy_render_widget_host_HWND_->SetBounds(
        window_->GetBoundsInRootWindow());
  }
#endif
  if (!in_shutdown_) {
    if (host_->delegate())
      host_->delegate()->SendScreenRects();
    else
      host_->SendScreenRects();
  }
}
