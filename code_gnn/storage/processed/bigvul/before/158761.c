void WebContentsImpl::CreateNewFullscreenWidget(int32_t render_process_id,
                                                int32_t route_id,
                                                mojom::WidgetPtr widget) {
  CreateNewWidget(render_process_id, route_id, true, std::move(widget),
                  blink::kWebPopupTypeNone);
}
