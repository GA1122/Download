void RenderViewImpl::ShowCreatedPopupWidget(RenderWidget* popup_widget,
                                            WebNavigationPolicy policy,
                                            const gfx::Rect& initial_rect) {
  Send(new ViewHostMsg_ShowWidget(GetRoutingID(), popup_widget->routing_id(),
                                  initial_rect));
}
