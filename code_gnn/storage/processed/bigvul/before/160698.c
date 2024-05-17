void RenderFrameImpl::PepperDidChangeCursor(
    PepperPluginInstanceImpl* instance,
    const blink::WebCursorInfo& cursor) {
  if (instance == pepper_last_mouse_event_target_)
    GetRenderWidget()->DidChangeCursor(cursor);
}
