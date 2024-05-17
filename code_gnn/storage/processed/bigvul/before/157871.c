void WebContentsImpl::ShowCreatedFullscreenWidget(int process_id,
                                                  int widget_route_id) {
  ShowCreatedWidget(process_id, widget_route_id, true, gfx::Rect());
}
