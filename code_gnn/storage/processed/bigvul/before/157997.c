void RenderViewImpl::ShowCreatedFullscreenWidget(
    RenderWidget* fullscreen_widget,
    WebNavigationPolicy policy,
    const gfx::Rect& initial_rect) {
  Send(new ViewHostMsg_ShowFullscreenWidget(GetRoutingID(),
                                            fullscreen_widget->routing_id()));
}
