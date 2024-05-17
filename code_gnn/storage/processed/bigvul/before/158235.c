void RenderWidgetHostImpl::ClearDisplayedGraphics() {
  NotifyNewContentRenderingTimeoutForTesting();
  if (view_) {
    if (enable_surface_synchronization_)
      view_->ResetFallbackToFirstNavigationSurface();
    else
      view_->ClearCompositorFrame();
  }
}
