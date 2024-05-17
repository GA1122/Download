void WebContentsImpl::ShowCreatedFullscreenWidget(int route_id) {
  ShowCreatedWidget(route_id, true, gfx::Rect());

  DCHECK_EQ(MSG_ROUTING_NONE, fullscreen_widget_routing_id_);
  fullscreen_widget_routing_id_ = route_id;
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    DidShowFullscreenWidget(route_id));
}
