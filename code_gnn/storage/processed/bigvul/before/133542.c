void WebContentsImpl::CreateNewFullscreenWidget(int render_process_id,
                                                int route_id) {
  CreateNewWidget(render_process_id, route_id, true, blink::WebPopupTypeNone);
}
