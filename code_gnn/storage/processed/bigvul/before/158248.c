void RenderWidgetHostImpl::DidUpdateVisualProperties(
    const cc::RenderFrameMetadata& metadata) {
  TRACE_EVENT0("renderer_host",
               "RenderWidgetHostImpl::DidUpdateVisualProperties");

  DCHECK(!metadata.viewport_size_in_pixels.IsEmpty());

  visual_properties_ack_pending_ = false;

  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DID_UPDATE_VISUAL_PROPERTIES,
      Source<RenderWidgetHost>(this), NotificationService::NoDetails());

  if (!view_ || is_hidden_)
    return;

  viz::ScopedSurfaceIdAllocator scoped_allocator =
      view_->DidUpdateVisualProperties(metadata);

  if (auto_resize_enabled_ && delegate_) {
    gfx::Size viewport_size_in_dip = gfx::ScaleToCeiledSize(
        metadata.viewport_size_in_pixels, 1.f / metadata.device_scale_factor);
    delegate_->ResizeDueToAutoResize(this, viewport_size_in_dip);
  }
}
