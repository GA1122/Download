void WebContentsImpl::CreateNewWidget(int32_t render_process_id,
                                      int32_t widget_route_id,
                                      mojom::WidgetPtr widget) {
  CreateNewWidget(render_process_id, widget_route_id,  false,
                  std::move(widget));
}