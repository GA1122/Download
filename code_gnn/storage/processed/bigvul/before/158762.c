void WebContentsImpl::CreateNewWidget(int32_t render_process_id,
                                      int32_t route_id,
                                      mojom::WidgetPtr widget,
                                      blink::WebPopupType popup_type) {
  CreateNewWidget(render_process_id, route_id, false, std::move(widget),
                  popup_type);
}
