void RenderMessageFilter::OnCreateFullscreenWidget(int opener_id,
                                                   int* route_id,
                                                   int* surface_id) {
  render_widget_helper_->CreateNewFullscreenWidget(
      opener_id, route_id, surface_id);
}
