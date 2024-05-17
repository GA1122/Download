void RenderViewHostImpl::ForwardMouseEvent(
    const WebKit::WebMouseEvent& mouse_event) {

  WebKit::WebMouseEvent event_copy(mouse_event);
  RenderWidgetHostImpl::ForwardMouseEvent(event_copy);

  switch (event_copy.type) {
    case WebInputEvent::MouseMove:
      delegate_->HandleMouseMove();
      break;
    case WebInputEvent::MouseLeave:
      delegate_->HandleMouseLeave();
      break;
    case WebInputEvent::MouseDown:
      delegate_->HandleMouseDown();
      break;
    case WebInputEvent::MouseWheel:
      if (ignore_input_events())
        delegate_->OnIgnoredUIEvent();
      break;
    case WebInputEvent::MouseUp:
      delegate_->HandleMouseUp();
    default:
      break;
  }
}
