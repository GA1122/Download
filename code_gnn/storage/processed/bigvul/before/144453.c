void WebContentsImpl::CreateNewWidget(int32_t render_process_id,
                                      int32_t route_id,
                                      blink::WebPopupType popup_type) {
  CreateNewWidget(render_process_id, route_id, false, popup_type);
}
