void WebContentsImpl::DidReceiveInputEvent(
    RenderWidgetHostImpl* render_widget_host,
    const blink::WebInputEvent::Type type) {
  if (!IsUserInteractionInputType(type))
    return;

  if (!HasMatchingWidgetHost(&frame_tree_, render_widget_host))
    return;

  if (type != blink::WebInputEvent::kGestureScrollBegin)
    last_interactive_input_event_time_ = ui::EventTimeForNow();

  for (auto& observer : observers_)
    observer.DidGetUserInteraction(type);

  if (IsResourceLoadUserInteractionInputType(type))
    SendUserGestureForResourceDispatchHost();
}
