RenderWidgetHostImpl* WebContentsImpl::GetFullscreenRenderWidgetHost() const {
  return RenderWidgetHostImpl::FromID(fullscreen_widget_process_id_,
                                      fullscreen_widget_routing_id_);
}
