void WebContentsImpl::CreateNewFullscreenWidget(int32_t render_process_id,
                                                int32_t route_id) {
  CreateNewWidget(render_process_id, route_id, true, blink::WebPopupTypeNone);
}
